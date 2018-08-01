#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "util.h"
#include "list.h"

#define TK_AGAIN EAGAIN           // 阻塞错误      

#define TK_HTTP_PARSE_INVALID_METHOD        10
#define TK_HTTP_PARSE_INVALID_REQUEST       11
#define TK_HTTP_PARSE_INVALID_HEADER        12

#define TK_HTTP_UNKNOWN                     0x0001
#define TK_HTTP_GET                         0x0002
#define TK_HTTP_HEAD                        0x0004
#define TK_HTTP_POST                        0x0008

#define TK_HTTP_OK                          200
#define TK_HTTP_NOT_MODIFIED                304
#define TK_HTTP_NOT_FOUND                   404
#define MAX_BUF 8124

typedef struct tk_http_request{
    char* root;                     // 配置文件
    int fd;                         // 描述符
    int epoll_fd;                   // epoll
    char buff[MAX_BUF];             // 用户缓冲
    size_t pos;                     // 缓冲区读取的当前位置
    size_t last;                    // 缓冲区的结束位置
    int state;                      // 请求头解析状态

    // 一下主要是解析请求时索引信息
    // 部分未使用
    void* request_start;
    void* method_end;
    int method;                     // 请求方法

    void* uri_start;
    void* uri_end;
    void* path_start;
    void* path_end;
    void* query_start;
    void* query_end;
    int http_major;
    int http_minor;
    void* request_end;

    struct list_head list;    // 存储请求头，list.h中定义了此结构

    void* cur_header_key_start;
    void* cur_header_key_end;
    void* cur_header_value_start;
    void* cur_header_value_end;
    void* timer;                // 时间结构
}tk_http_request_t;


// 响应头结构
typedef struct tk_http_out{    
    int fd;                     // 连接描述符
    int keep_alive;             // http连接状态
    time_t mtime;               // 文件类型
    int modified;               // 是否修改
    int status;                 // 返回码
}tk_http_out_t;


// http头部结构体
typedef struct tk_http_header{
    void* key_start;            // 应答头
    void* key_end;
    void* value_start;          // value
    void* value_end;
    struct list_head list;      // 链表
}tk_http_header_t;


// 头部处理函数
typedef int (*tk_http_header_handler_pt)(tk_http_request_t* request, tk_http_out_t* out, char* data, int len);

// 头部处理结构体
typedef struct tk_http_header_handle{
    char* name;
    tk_http_header_handler_pt handler;    // 函数指针
}tk_http_header_handle_t;

extern tk_http_header_handle_t tk_http_headers_in[];


/*  请求响应 函数 声明  */

// 头部处理函数
void tk_http_handle_header(tk_http_request_t* request, tk_http_out_t* out);

// 关闭连接
int tk_http_close_conn(tk_http_request_t* request);


// 获取状态码提示
const char* get_shortmsg_from_status_code(int status_code);


// 初始化请求头结构
int tk_init_request_t(tk_http_request_t* request, int fd, int epoll_fd, char* path);

// 初始化响应头结构
int tk_init_out_t(tk_http_out_t* out, int fd);


//  删除请求头结构
int tk_free_request_t(tk_http_request_t *request);


// 删除响应头部结构
int tk_free_out_t(tk_http_out_t *out);




#endif