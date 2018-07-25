#include< stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>   // inet_aton, inet_addr inet_ntoa
#include<unistd.h>
#include<ctype.h>
#include<string.h>
#include<strings.h>
#include<sys/stat.h>
#include<sys/stat.h>
#include<pthread.h>
#include < sys/wait.h>
#include<stdlib.h>

// 宏定义
// 是否空格
#define ISspace(x) isspace((int)(x))

#define SERVER_STRING "Server: jdhhtpd/0.1.0/r/n"

/* 函数声明 */
// 每次收到请求，创建一个线程来处理接受到的请求
// 把client_sock地址转为参数传送给pthread_create
void accept_request(void *arg);


// 错误请求
void bad_request(int);

// 读取文件
void cat(int ,FILE*);

// 无法执行
void cannot_excute(int);


// 错误输出
void error_die(const char*);

// 执行cgi脚本
void execute_cgi(int ,const char* , const char*, const char*);

// 解析一行数据
void get_line(int ,char* int);

// 返回http头部
void headers(int , const char*)；

// 没有发现文件
void not_found(int);


// 如果不是cgi文件， 直接读取文件返回给请求的http客户端；
void serve_file(int ,const char*);

// 开启tcp连接，绑定端口
int startup(u_short *);

// 如果不是get或者post， 其他方法没有实现
void unimplemented(int);


/*  函数 实现  */

void accept_request(void *arg){
	int client = (intptr_t)arg;
	char buf[1024];
	int numchars;
	char method[255];
	char url[255];
	char paht[512];
	
	size_t i,j ;
	struct stat st;   // 文件状态
	int cgi = 0;
	char *query_string = NULL:
	
	// 取第一行 http请求
	numchars = get_line(client, buf, sizeof(buf));
	i=0,j=0;
	
	// http请求方法
	while(!ISspace(buf[j]) && (i < sizeof(method) -1)){
		method[i] = buf[j];
		i++;
		j++;
	}
	
	// 空格
	method[i] = '\0';
	// 判断get或者post
	if(strcasecmp(method, "GET") && strcasecmp(method,"POST")) // 不为get，post
	{
		unimplemented(client);
		return ;
	}
	
	// post cgi设置为1
	if(strcasecmp(method,"POST") == 0){
		cgi = 1;
	}
	
	i = 0; 
	// 跳过空格
	while(ISspace(buf[j])&& (j<sizeof(buf)))
		j++;
	
	// 解析path 例如：/index.html
	// GET /index.html HTTP/1.1
	while(!ISspace(buf[j]) && (i< sizeof(url) -1) &&(j <sizeof(buf))){
		url[i] = buf[j];
		i++;
		j++;
	}
	url[i] ='\0';  // 添加结尾
	
	/* 判断 get  请求 */
	if(strcasecmp  )
	
}








































