#include <stdbool.h>
#include "convert.h"
#include "search_path.h"
#include "queue.h"
#include "coding.h"

queue_root_t *q_root;

int main(int argc, char *argv[])
{
    const char *suffix[] = {".c", ".h"};
    if (argc < 2) {
        perror("参数过少：");
        printf("%s <源文件>", argv[0]);
        return -1;
    }

    /* 初始化队列 */
    q_root = queue_root_init();

    /* 检测传入的路径是一个文件还是文件夹 */
    if (is_directory(argv[1]) != 1) {
        convert(argv[1]);
    } else {
        walk(argv[1], suffix, ROW_COUNT(suffix));

        queue_node_t *path_node = NULL;
        while ((path_node = dequeue(q_root)) != NULL) {
            convert(path_node->path);
        }
    }

    printf("文件/文件夹 %s 转化完成\n", argv[1]);

    return 0;
}