#ifndef UTIL_H
#define UTIL_H

#define PATHLEN 128
#define LISTENQ 1024
#define BUFLEN 8192
#define DELIM "="


#define TK_CONF_OK 0
#define TK_CONF_ERROR -1

#define MIN(a,b) ((a)<(b) ? (a) : (b))


// 配置文件结构体
typedef struct tk_conf
{
	char root[PATHLEN];			// 文件根目录
	int port;					// 端口号
	int thread_num;				// 线程数
}tk_conf_t;



/*  util 相关函数声明  */
int read_conf(char *filename, tk_conf_t *conf);
void handle_for_sigpipe();
int socket_bind_listen(int port);
int make_socket_non_blocking(int fd);
void accept_connection(int listen_fd, int epoll_fd, char *path);




#endif