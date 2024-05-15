#include "convert.h"

int gbk_to_utf8(char *inbuf, size_t *inlen, char *outbuf, size_t *outlen)
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

int utf8_to_gbk(char *inbuf, size_t *inlen, char *outbuf, size_t *outlen)
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