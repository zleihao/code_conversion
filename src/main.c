#include "convert.h"
#include "search_path.h"

char origin_buff[2048];
char convert[2048];

int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    FILE *target = NULL;
    size_t ret = 0, srclen, outlen = 2048;

    if (argc < 3) {
        perror("参数过少：");
        printf("%s <源文件> <目标文件>", argv[0]);
        return -1;
    }

    walk("/mnt/d/study/code_conversion/");

    exit(0);

    target = fopen(argv[2], "w+");

    fp = fopen(argv[1], "r");
    if (NULL == fp) {
        perror("文件打开失败\n");
        return -1;
    }

    // 逐行读取文件内容
    while (fgets(origin_buff, 2048, fp) != NULL) {

        /* 得到一行内容的长度 */
        srclen = strlen(origin_buff);

        //进行字符编码转换，GBK ---> UTF-8
        // ret = gbk_to_utf8(origin_buff, &srclen, convert, &outlen);
        ret = utf8_to_gbk(origin_buff, &srclen, convert, &outlen);
        if (-1 == ret) {
            return -1;
        }

        //将转换后的文件写到新的文件中
        fwrite(convert, 1, strlen(convert), target);
        fflush(target);

        //清空转换结果缓冲区
        memset(convert, 0, sizeof(convert));
        outlen = sizeof(convert);
    }

    //关闭文件句柄
    fclose(fp);
    fclose(target);

    return 0;
}