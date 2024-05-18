#include "convert.h"
#include "search_path.h"
#include "queue.h"

queue_root_t *q_rooot;

int main(int argc, char *argv[])
{
    // 释放内存等其他操作
    const char *suffix[] = {".c", ".h"};
    if (argc < 2) {
        perror("参数过少：");
        printf("%s <源文件>", argv[0]);
        return -1;
    }

    convert(argv[1]);
    return 0;
}