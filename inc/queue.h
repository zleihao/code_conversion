#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 队列节点 */
typedef struct queue_node {
    struct queue_node *prev;
    struct queue_node *next;
    //文件路径
    char path[1024];
} queue_node_t;

/* 队列头部 */
typedef struct {
   queue_node_t *head;
   queue_node_t *tail;
} queue_root_t;


/* API */
queue_root_t *queue_root_init(void);
queue_node_t *queue_node_create(const char *str);
int enqueue(queue_root_t *q_root, queue_node_t *q_node);
queue_node_t *dequeue(queue_root_t *q_root);
void print_queue(queue_root_t *q_root);

#endif