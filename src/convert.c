#include "convert.h"
#include "coding.h"
#include <stdio.h>
#include <iconv.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

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

uint32_t get_file_size(FILE *fp)
{
    uint32_t f_size = 0;

    //得到文件长度
    fseek(fp, 0, SEEK_END);
    f_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return f_size;
}

char *read_file(FILE *fp, uint32_t f_size)
{
    char *des = NULL;

    des = (char *)calloc((f_size + 4), sizeof(char));
    if (NULL == des) {
        return NULL;
    }

    // 读取源文件内容
    if (fread(des, 1, f_size, fp) != f_size) {
        return NULL;
    }

    return des;
}

/**
 * @brief 将指定的文件编码由gbk转成utf-8
 * @param path
 * @return
 */
int convert(const char *path)
{
    // 释放内存等其他操作
    FILE *fp = NULL;
    FILE *target = NULL;
    uint32_t file_size = 0;
    char *origin = NULL, *des = NULL;
    size_t ret = 0, srclen, outlen;

    if (!is_file_gbk(path)) {
        printf("%s It is not a gbk code and does not support conversion\n", path);
        return -1;
    }
    //备份文件
    char *back_file = NULL;
    back_file = (char *)malloc(strlen(path) + 6);
    if (NULL == back_file) {
        printf("malloc fail\n");
        return -1;
    }

    strcpy(back_file, path);
    strcat(back_file, ".back");

    target = fopen(back_file, "wb+");
    if (NULL == target) {
        printf("create tmp file fail!\n");
        free(back_file);
        return -1;
    }

    fp = fopen(path, "rb");
    if (NULL == fp) {
        printf("%s open fail\n", path);
        fclose(target);
        remove(back_file);
        free(back_file);
        return -1;
    }

    file_size = get_file_size(fp);

    des = (char *)calloc((file_size*4) + 4, 1);
    if (NULL == des) {
        goto FAIL_CLEAN_FILE;
    }

    // 读取源文件内容
    origin = read_file(fp, file_size);
    if (NULL == origin) {
        fprintf(stderr, "read file %s fail!\n", path);
        goto FAIL_CLEAN_FILE;
    }

    //进行字符编码转换，GBK ---> UTF-8
    srclen = file_size;
    outlen = file_size * 4;
    ret = gbk_to_utf8(origin, &srclen, des, &outlen);
    if (-1 == ret) {
        goto FAIL_CLEAN_FILE;
    }

    // 将转换后的内容写入目标文件
    if (fwrite(des, 1, strlen(des), target) != strlen(des)) {
        perror("writer file fail!");
        goto FAIL_CLEAN_FILE;
    }
    fflush(target);

    //关闭文件句柄
    fclose(fp);
    fclose(target);

    // 删除源文件
    if (remove(path) != 0) {
        printf("del file %s fail", path);
        remove(back_file);
        goto FAIL_FREE_MEM;
    }

    // 重命名目标文件为源文件名
    if (rename(back_file, path) != 0) {
        perror("file rename fail!");
        remove(back_file);
        goto FAIL_FREE_MEM;
    }

    /* 释放掉备份文件路径 */
    free(back_file);
    free(origin);
    free(des);

    return 0;

FAIL_CLEAN_FILE:
    fclose(fp);
    fclose(target);
    remove(back_file);
FAIL_FREE_MEM:
    free(back_file);
    free(origin);
    free(des);
    return -1;
}
