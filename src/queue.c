#include "queue.h"

/**
 * @brief  队列根初始化
 * @param  NULL
 * @return 队列根节点，queue_root_t *
*/
queue_root_t *queue_root_init(void)
{
    queue_root_t *new_queue_root = NULL;

    new_queue_root = (queue_root_t *)malloc(sizeof(queue_root_t));
    if (NULL == new_queue_root) {
        printf("queue root malloc dail!\n");
        return NULL;
    }

    new_queue_root->head = NULL;
    new_queue_root->tail = NULL;

    return new_queue_root;
}

/* 初始化队列节点 */
queue_node_t *queue_node_create(const char *str)
{
    queue_node_t *new_node = (queue_node_t *)malloc(sizeof(queue_node_t));

    if (NULL == new_node) {
        perror("create new node fail!\n");
        return NULL;
    }

    new_node->next = NULL;
    new_node->prev = NULL;
    strcpy(new_node->path, str);

    return new_node;
}
/**
 * @brief 从队尾入队
 * @param q_root 哪个队
 * @param q_node 入队的节点
 * @return 0 成功，-1 失败
 */
int enqueue(queue_root_t *q_root, queue_node_t *q_node)
{
    if (NULL == q_root) {
        perror("queue is empty, insert fail!\n");
        return -1;
    }

    q_node->prev = q_root->tail;
    q_node->next = NULL;

    if (q_root->tail != NULL) {
        q_root->tail->next = q_node;
    } else {
        q_root->head = q_node;
    }
    q_root->tail = q_node;

    return 0;
}

/* 出队 */
/**
 * @brief 从队头删除一个节点
 * @param q_root 哪个队
 * @return 队节点
 */
queue_node_t *dequeue(queue_root_t *q_root)
{
    queue_node_t *q_node = NULL;

    if (NULL == q_root->head) {
        return NULL;
    }

    if (q_root->tail != q_root->head) {
        q_node = q_root->head;
        q_root->head = q_root->head->next;
    } else {
        q_node = q_root->head;
        q_root->head = NULL;
        q_root->tail = NULL;
    }

    return q_node;
}

/* 打印队列 */
void print_queue(queue_root_t *q_root)
{
    if (NULL == q_root || NULL == q_root->head) {
        printf("queue is empty, nothing to print!\n");
        return;
    }

    queue_node_t *current = q_root->head;
    while (current != NULL) {
        printf("%s\n", current->path);
        current = current->next;
    }
}