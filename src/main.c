#include <stdbool.h>
#include "convert.h"
#include "search_path.h"
#include "queue.h"
#include "coding.h"

queue_root_t *q_rooot;

int main(int argc, char *argv[])
{
    const char *suffix[] = {".c", ".h"};
    if (argc < 2) {
        perror("参数过少：");
        printf("%s <源文件>", argv[0]);
        return -1;
    }

    //if (!is_file_gbk(argv[1])) {
    //    printf("文件 %s 不是一个gbk编码，暂不支持转化\n", argv[1]);
    //    return -1;
    //}
    convert(argv[1]);
    return 0;
}