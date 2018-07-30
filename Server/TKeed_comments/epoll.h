#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include "http.h"
#include "threadpool.h"

#define MAXEVENTS 1024

/* epoll 函数声明 */
int tk_epoll_create(int flags);

int tk_epoll_add(int epollfd, int fd, tk_http_request_t* request, int events);

int tk_epoll_mod(int epollfd, int fd, tk_http_request_t *request, int events);

int tk_epoll_del(int epollfd, int fd ,tk_http_request_t *request, int events);

int tk_epoll_wait(int epollfd, struct epoll_event *events, int max_events, int timeout);


void tk_handle_events(int epollfd, int listen_fd, struct epoll_event *events, 
		int events_num, char *path, tk_threadpool_t *tp);

#endif