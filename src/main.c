#include <stdbool.h>
#include "convert.h"
#include "search_path.h"
#include "queue.h"
#include "coding.h"
#include <pthread.h>
#include <unistd.h>

queue_root_t *q_root;
pthread_mutex_t f_lock;

#define NUM_THREAD 10

void *handler(void *arg) {

    while (1) {
        pthread_mutex_lock(&f_lock);
        queue_node_t *path_node = NULL;
        path_node = dequeue(q_root);
        pthread_mutex_unlock(&f_lock);

        if (path_node != NULL) {
            convert(path_node->path);
            usleep(5);
        } else {
            return NULL;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        perror("参数过少：");
        printf("%s <文件/文件夹>", argv[0]);
        return -1;
    }
    pthread_t tdi[NUM_THREAD];
    const char *suffix[] = {".c", ".h"};

    pthread_mutex_init(&f_lock, NULL);

    /* 初始化队列 */
    q_root = queue_root_init();

    /* 检测传入的路径是一个文件还是文件夹 */
    if (is_directory(argv[1]) != 1) {
        convert(argv[1]);
    } else {
        walk(argv[1], suffix, ROW_COUNT(suffix));
        for (int i = 0; i < NUM_THREAD; i++) {
            pthread_create(&tdi[i], NULL, handler, NULL);
        }

        for (int i = 0; i < NUM_THREAD; i++) {
            pthread_join(tdi[i], NULL);
        }
    }

    printf("文件/文件夹 %s 转换完成\n", argv[1]);

    pthread_mutex_destroy(&f_lock);

    return 0;
}