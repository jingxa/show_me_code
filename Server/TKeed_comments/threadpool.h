#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

// 任务结构体 
typedef struct tk_task{
    void (*func)(void*);
    void* arg;
    struct tk_task* next;    // 任务链表（下一节点指针）
}tk_task_t;


// 线程池结构体
typedef struct threadpool{
    pthread_mutex_t lock;    // 互斥锁
    pthread_cond_t cond;    // 条件变量
    pthread_t *threads;    // 线程
    tk_task_t *head;    // 任务链表
    int thread_count;    // 线程数 
    int queue_size;    // 任务链表长
    int shutdown;     // 关机模式
    int started;
}tk_threadpool_t;

// 线程错误状态
typedef enum{
    tk_tp_invalid = -1,
    tk_tp_lock_fail = -2,
    tk_tp_already_shutdown = -3,
    tk_tp_cond_broadcast = -4,
    tk_tp_thread_fail = -5,
}tk_threadpool_error_t;


// 线程池关闭状态
typedef enum{
    immediate_shutdown = 1,
    graceful_shutdown = 2
}tk_threadpool_sd_t;



/* 线程池 函数 声明*/

// 初始化线程池
tk_threadpool_t* threadpool_init(int thread_num);

//添加任务
int threadpool_add(tk_threadpool_t* pool, void (*func)(void *), void* arg);

// 回收线程池资源
int threadpool_destroy(tk_threadpool_t* pool, int gracegul);

// 释放线程池及任务


// 工作线程


#endif
