#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdlib.h>

#define TK_PQ_DEFAULT_SIZE 10

typedef int (*tk_pq_comparator_pt)(void *pi, void *pj);

typedef struct priority_queue{
    void **pq;				// 优先队列指针	
    size_t nalloc;			// 优先队列实际元素个数
    size_t size;			// 优先队列大小
    tk_pq_comparator_pt comp;	// 堆模式
}tk_pq_t;


/*	优先队列的函数声明  */

// 初始化优先队列
int tk_pq_init(tk_pq_t *tk_pq, tk_pq_comparator_pt comp, size_t size);

// 判断优先队列为空
int tk_pq_is_empty(tk_pq_t *tk_pq);

// 队列大小
size_t tk_pq_size(tk_pq_t *tk_pq);

// 队列最小的元素
void *tk_pq_min(tk_pq_t *tk_pq);

// 删除队列最小的元素
int tk_pq_delmin(tk_pq_t *tk_pq);

// 插入一个元素
int tk_pq_insert(tk_pq_t *tk_pq, void *item);


// 调整存储空间
int tk_pq_sink(tk_pq_t *tk_pq, size_t i);



#endif