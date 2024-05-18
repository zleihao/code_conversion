#include "convert.h"

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
    if (ret == -1) {
        perror("iconv fail\n");
        return -1;
    }

    /* 关闭句柄 */
    iconv_close(icp);

    return 0;
}

static int utf8_to_gbk(char *inbuf, size_t *inlen, char *outbuf, size_t *outlen)
{
    /* 目的编码, TRANSLIT：遇到无法转换的字符就找相近字符替换
     *           IGNORE ：遇到无法转换字符跳过*/
    char *encTo = "GBK";
    /* 源编码 */
    char *encFrom = "UTF-8";

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
    if (ret == -1) {
        perror("iconv fail\n");
        return -1;
    }
    
    /* 关闭句柄 */
    iconv_close(icp);

    return 0;
}

int convert(char *path)
{
    // 释放内存等其他操作
    FILE *fp = NULL;
    FILE *target = NULL;

    char origin_buff[2049];
    char des[2049];

    //备份文件
    char *back_file = NULL;
    back_file = (char *)malloc(strlen(path) + 6);

    size_t ret = 0, srclen, outlen = 2048;

    strcat(back_file, path);
    strcat(back_file, ".back");

    target = fopen(back_file, "w+");

    fp = fopen(path, "r");
    if (NULL == fp) {
        printf("文件 %s 打开失败\n", path);
        return -1;
    }

    // 逐行读取文件内容
    while (fgets(origin_buff, 2048, fp) != NULL) {

        /* 得到一行内容的长度 */
        srclen = strlen(origin_buff);

        //进行字符编码转换，GBK ---> UTF-8
        ret = gbk_to_utf8(origin_buff, &srclen, des, &outlen);
        //ret = utf8_to_gbk(origin_buff, &srclen, convert, &outlen);
        if (-1 == ret) {
            return -1;
        }

        //将转换后的文件写到新的文件中
        fwrite(des, 1, strlen(des), target);
        fflush(target);

        //清空转换结果缓冲区
        memset(des, 0, sizeof(des));
        outlen = sizeof(des);
    }

    // 删除源文件
    if (remove(path) != 0) {
        perror("删除源文件失败");
        return -1;
    }

    // 重命名目标文件为源文件名
    if (rename(back_file, path) != 0) {
        perror("重命名文件失败");
        return -1;
    }

    //关闭文件句柄
    fclose(fp);
    fclose(target);

    return 0;
}