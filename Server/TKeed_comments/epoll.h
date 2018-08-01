#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include "http.h"
#include "threadpool.h"

#define MAXEVENTS 1024

/* epoll 函数声明 */
// 创建epoll
int tk_epoll_create(int flags);

// 添加epoll 事件
int tk_epoll_add(int epollfd, int fd, tk_http_request_t* request, int events);

// 修改事件状态
int tk_epoll_mod(int epollfd, int fd, tk_http_request_t *request, int events);


//从epoll中删除
int tk_epoll_del(int epollfd, int fd ,tk_http_request_t *request, int events);

// 等待事件
int tk_epoll_wait(int epollfd, struct epoll_event *events, int max_events, int timeout);

// 分发对应事件
void tk_handle_events(int epollfd, int listen_fd, struct epoll_event *events, 
		int events_num, char *path, tk_threadpool_t *tp);

#endif