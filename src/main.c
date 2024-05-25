#include "convert.h"
#include "search_path.h"
#include "queue.h"
#include "coding.h"
#include <pthread.h>
#include <stdint.h>
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

/* 解析命令行 */
char **parse_argc(char *argc, uint32_t *str_size)
{
    uint32_t word_count = 1;

    char **ans;
    char *word = NULL;

    // 跳过前导空格
    while (*argc == ' ') {
        argc++;
    }
    /* 统计单词个数 */
    for (int i = 0; argc[i]; i++) {
        if (argc[i] == ' ') {
            word_count++;
        }
    }

    ans = (char **)malloc(sizeof(char *) * (word_count + 1));
    if (NULL == ans) {
        return NULL;
    }

    word = strtok(argc, " ");
    int i = 0;
    while (word != NULL) {
        ans[i] = malloc(strlen(word) + 1);
        if (ans[i] == NULL) {
            // 内存分配失败,释放已分配的内存
            for (int j = 0; j < i; j++) {
                free(ans[j]);
            }
            free(ans);
            return NULL;
        }
        strcpy(ans[i], word);
        word = strtok(NULL, " ");
        i++;
    }
    ans[i] = NULL; // 添加结束标记
    *str_size = word_count;

    return ans;
}

int main(int argc, char *argv[])
{
    if (argc > 3) {
        printf("Usage is wrong: %s <file path> <file suffix list>", replace_backslashes_with_slashes(argv[0]));
        return -1;
    }

    pthread_t tdi[NUM_THREAD];
    pthread_mutex_init(&f_lock, NULL);

    /* 初始化队列 */
    q_root = queue_root_init();

    /* 检测传入的路径是一个文件还是文件夹 */
    if (is_directory(argv[1]) != 1) {
        convert(argv[1]);
    } else {
        if (NULL == argv[2]) {
            printf("Usage is wrong: %s <file path> <file suffix list>", replace_backslashes_with_slashes(argv[0]));
            return -1;
        }
        uint32_t size = 0;
        char **suffix = parse_argc(argv[2], &size);
        if (NULL == suffix) {
            printf("file suffix list error!\n");
            return -1;
        }
        walk(replace_backslashes_with_slashes(argv[1]), suffix, size);
        for (int i = 0; i < NUM_THREAD; i++) {
            pthread_create(&tdi[i], NULL, handler, NULL);
        }

        for (int i = 0; i < NUM_THREAD; i++) {
            pthread_join(tdi[i], NULL);
        }
    }

    printf("%s transcoding complete\n", argv[1]);

    pthread_mutex_destroy(&f_lock);

    return 0;
}