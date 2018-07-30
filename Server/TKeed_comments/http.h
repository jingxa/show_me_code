#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>   // 
#include <unistd.h>
#include <sys/stat.h>   //
#include <sys/types.h>
#include < fcntl.h>
#include "timer.h"
#include "util.h"
#include "rio.h"
#include "epoll.h"
#include "http_parese.h"
#include "http_request.h"


// 头缓冲（512 字节 ）和数据缓冲（8192字节）
#define MAXLINE  8192   
#define SHORTLINE  512   


#define tk_str3_cmp(m,c0,c1,c2,c3)									\
	*(uint32_t *) m == ((c3<<24) | (c2 << 16) | (c1<< 8) |c0)
#define tk_str3Ocmp(m,c0,c1,c2,c3)									\
	*(uint32_t *) m == ((c3<<24) | (c2 << 16) | (c1<< 8) |c0)	
#define tk_str4cmp(m,c0,c1,c2,c3)									\
	*(uint32_t *) m == ((c3<<24) | (c2 << 16) | (c1<< 8) |c0)

// 用key_value 表示mime_type_t
typedef struct  mime_type       // htttp 请求头部的mime
{
	const char* type;
	const char* value;
	
}mime_type_t;  


void do_request(void* prt);  // 解析


#endif  // HTTP_H