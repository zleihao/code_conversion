#include "convert.h"
#include "coding.h"

static int gbk_to_utf8(char *inbuf, size_t *inlen, char *outbuf, size_t *outlen)
{
    /* 目的编码, TRANSLIT：遇到无法转换的字符就找相近字符替换
     *           IGNORE ：遇到无法转换字符跳过*/
    char *encTo = "UTF-8//IGNORE";
    /* 源编码 */
    char *encFrom = "GBK";

    /**
     * @brief 获得转换句柄
     * @param encTo   目标编码方式
     * @param encFrom 源编码方式
     */
    iconv_t icp = iconv_open(encTo, encFrom);
    if ((iconv_t)-1 == icp)
    {
        perror("iconv_open fail!\n");
        return -1;
    }

    /* 由于iconv()函数会修改指针，所以要保存源指针 */
    char *tmpin = inbuf;
    char *tmpout = outbuf;
    size_t insize = *inlen;
    size_t outsize = *outlen;

    /* 进行转换
     *@param icp iconv_open()产生的句柄
     *@param srcstart 需要转换的字符串
     *@param inlen 存放还有多少字符没有转换
     *@param tempoutbuf 存放转换后的字符串
     *@param outlen 存放转换后,tempoutbuf剩余的空间
     *
     * */
    size_t ret = iconv(icp, &tmpin, inlen, &tmpout, outlen);
    if (ret == (size_t)-1) {
        if (errno == E2BIG) {
            // 输出缓冲区空间不足
            fprintf(stderr, "Output buffer too small\n");
            iconv_close(icp);
            return -1;
        } else if (errno == EILSEQ || errno == EINVAL) {
            // 非法的多字节序列或不完整的多字节序列
            fprintf(stderr, "Invalid or incomplete multibyte sequence\n");
            iconv_close(icp);
            return -1;
        } else {
            // 其他错误
            perror("iconv fail");
            iconv_close(icp);
            return -1;
        }
    }

    /* 关闭句柄 */
    iconv_close(icp);
    // 更新输出长度
    *outlen -= outsize;

    return 0;
}

int convert(const char *path)
{
    // 释放内存等其他操作
    FILE *fp = NULL;
    FILE *target = NULL;
    uint32_t file_size = 0;
    char *origin = NULL, *des = NULL;
    size_t ret = 0, srclen, outlen;

    if (!is_file_gbk(path)) {
        printf("%s 不是一个gbk编码，暂不支持转化\n", path);
        return -1;
    }
    //备份文件
    char *back_file = NULL;
    back_file = (char *)malloc(strlen(path) + 6);
    if (NULL == back_file) {
        printf("申请空间失败\n");
        return -1;
    }

    strcpy(back_file, path);
    strcat(back_file, ".back");

    target = fopen(back_file, "w+");
    if (NULL == target) {
        printf("临时文件创建失败\n");
        free(back_file);
        return -1;
    }

    fp = fopen(path, "r");
    if (NULL == fp) {
        printf("%s 打开失败\n", path);
        fclose(target);
        free(back_file);
        return -1;
    }


    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    origin = (char *)calloc(file_size + 4, 1);
    if (NULL == origin) {
        fclose(fp);
        fclose(target);
        free(back_file);
        return -1;
    }

    des = (char *)calloc((file_size*4) + 4, 1);
    if (NULL == des) {
        fclose(fp);
        fclose(target);
        free(back_file);
        free(origin);
        return -1;
    }

    // 读取源文件内容
    if (fread(origin, 1, file_size, fp) != file_size) {
        perror("文件读取失败");
        fclose(fp);
        fclose(target);
        free(back_file);
        free(origin);
        free(des);
        return -1;
    }

    //进行字符编码转换，GBK ---> UTF-8
    srclen = file_size;
    outlen = file_size * 4;
    ret = gbk_to_utf8(origin, &srclen, des, &outlen);
    if (-1 == ret) {
        fclose(fp);
        fclose(target);
        remove(back_file);
        free(back_file);
        free(des);
        free(origin);
        return -1;
    }

    // 将转换后的内容写入目标文件
    if (fwrite(des, 1, strlen(des), target) != strlen(des)) {
        perror("文件写入失败");
        fclose(fp);
        fclose(target);
        remove(back_file);
        free(back_file);
        free(origin);
        free(des);
        return -1;
    }
    fflush(target);

    //关闭文件句柄
    fclose(fp);
    fclose(target);

    // 删除源文件
    if (remove(path) != 0) {
        printf("删除源文件%s失败", path);
        free(back_file);
        free(origin);
        free(des);
        return -1;
    }

    // 重命名目标文件为源文件名
    if (rename(back_file, path) != 0) {
        perror("重命名文件失败");
        free(back_file);
        free(origin);
        free(des);
        return -1;
    }

    /* 释放掉备份文件路径 */
    free(back_file);
    free(origin);
    free(des);

    return 0;
}