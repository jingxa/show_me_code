#ifndef TK_TIMER_H
#define TK_TIMER_H

#include "priority_queue.h"
#include "http_request.h"


#define TIMEOUT_DEFAULT 500 	/*ms*/

// 函数指针，负责超时处理， tk_add_timer时指定处理函数
typedef int (*timer_handler_pt)(tk_http_request_t *request);

typedef struct tk_timer{
	size_t key;   		// 标记超时时间
	int deleted; 		// 标记是否被删除
	timer_handler_pt handler;		// 超时处理函数，add的时候添加
	tk_http_request_t *request;		// 指向对应的request请求
} tk_timer_t;



// tk_pq_t 定义在“priority_queue.h”中， 优先队列中的节点
extern tk_pq_t tk_timer;
extern size_t	tk_current_msec;   


/*  定时器相关函数 */

//初始化时间
int tk_timer_init();

// 寻找时间戳
int tk_find_timer();

// 处理超时
void tk_handle_expire_timers();

// 新增时间戳
void tk_add_timer(tk_http_request_t *request, size_t timeout, timer_handler_pt handler);

// 删除时间戳
void tk_del_timer(tk_http_request_t *request);

// 时间戳比较
int timer_comp(void *ti, void *tj);



#endif