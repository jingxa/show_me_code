#ifndef RIO_H
#define RIO_H


#include <sys/types.h>
#define RIO_BUFSIZE 8192


typedef struct
{
	int rio_fd;   			// 内部缓冲描述符
	ssize_t rio_cnt;		// 缓冲区未读字节总数
	char *rio_bufptr;		// 指向缓冲区下一个未读的字节
	char rio_buf[RIO_BUFSIZE];  // 内部缓冲区
} rio_t;


/*  RIO  函数 声明  */
//
ssize_t rio_readn(int fd, void *userbuf, size_t n);

//
ssize_t rio_writen(int fd, void *userbuf, size_t n);

//
void rio_readinitb(rio_t *rp, int fd);

//
ssize_t rio_readnb(rio_t *rp , void *userbuf, size_t n);

//
ssize_t rio_readlineb(rio_t *rp, void *userbuf, size_t maxlen);


#endif