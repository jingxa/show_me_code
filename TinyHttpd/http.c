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
	if(strcasecmp(method, "GET") == 0  ){
		query_string  = url;
		while((*query_string!='?') && (*query_string != '\0'))
			query_string ++;
		if(*query_string == '?')
		{
			 //如果是 ？ 的话，证明这个请求需要调用 cgi，将 cgi 标志变量置一(true)
			cgi = 1;
			*query_string = '\0'; // 吧url分成两块
			query_string++;
		}
	}
	
	// 路径，将url的第一部分输出
	sprintf(path, "htdocs%s",url);
	// 默认地址，解析路径如果为/，自动添加index.html
	if(path[strlen(path) -1] == '/')
		strcat(path, "index.html");
	
	// 获得文件信息
	if(stat(path, &st) == -1){
		// 文件不存在， 把所有http信息读取然后丢弃
		while((numchars>0) && strcmp("\n",buf))
			numchars = get_line(client, buf, sizeof(buf));
		// 返回客户端 回应错误
		not_found(client);
	}else{
		// 文件存在，和常量相与， 判断文件类型
		if(st.st_mode & S_IFMT) == S_IFDIR)   // 目录
			strcat(path, "/index.html");
			
		// S_IXUSR :用户执行权限
		// S_IXGRP : 用户组成员执行权限
		// S_IXOTH: 其他用户执行权限
		// 文件可执行
		if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) 
			||(st.st_mode&S_IXOTH)){
				cgi = 1;
				if(! cgi)
					// 入耳不需要cgi
					serve_file(client, path);
				else
					execute_cgi(client, path, method, query_string);
			}
	}
	close(client);
	
}



/*	badd_request : 回应错误  */
void bad_request(int client){
	char buf[1024];
	
	sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "<P>Your browser sent a bad request, ");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "such as a POST without a Content-Length.\r\n");
	send(client, buf, sizeof(buf), 0);
	}

/* cat： 读取文件发送到socket */
void cat(int client, FILE* resource){
	char buf[1024];
	// 从文件描述符中读取内容
	fgets(buf, sizeof(buf), resource);
	while(!feof(resource)){
		send(client,buf, sizeof(buf),0);
		fgets(buf, sizeof(buf),resource);
	}
}

/* cannot_excute: 不能执行cgi */
void cannot_excute(int client){
	char buf[1024];
	
	sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<P>Error prohibited CGI execution.\r\n");
	send(client, buf, strlen(buf), 0);
}


/* error_die : 打印错误消息,然后退出  */ 
void error_die(const char* sc){
	perror(sc);
	exit(1);
}


/*	execute_cgi: 执行cgi */
void execute_cgi(int client, const char* path, 
	const char* method, const char* query_string){
	
	char buf[1024];
	int cgi_output[2];
	int cgi_input[2];
	pid_t pid; 
	int status;
	int i;
	char c;
	int numchars= 1;
	int content_length = -1;
	
	// 在buf中填东西保证进入while
	buf[0] =  'A'; buf[1] = '\0';
	// 如果是http请求是get方法的话，读取并忽略请求剩下的东西
	if(strcasecmp(method , "GET") == 0){
		while((numchars > 0) && strcmp("\n", buf))
			numchars = get_line(client, buf,sizeof(buf));
	}else{
		// post方法
		numchars = get_line(client,buf ,sizeof(buf)); // 读取第二行
		
		// 读取header部分
		while((numchars > 0) &&strcmp("\n", buf)){
			buf[15] = '\0';  // "Content-Length:" 后面添加'\0'
			if(strcasecmp(buf,"Content-Length:")== 0)
				content_length = atoi(&(buf[16]))); // body 长度
			numchars = get_line(client,buf,sizeof(buf));
		}
		
		// 如果http请求的header没有致使body长度，报错返回
		if(content_length == -1){
			bad_request(client);
			return;
		}
	}
	
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	send(client,buf,strlen(buf),0);
	
	// 下面创建两个管道，用于两个进程通信
	if(pipe(cgi_output)<0){
		cannot_excute(client);
		return;
	}
	
	if(pipe(cgi_input)<0){
		cannot_excute(client);
		return;
	}
	
	// 创建一个子进程
	if((pid = fork()) <0){// 失败
	cannot_excute(client);
		return ;
	}	
	
	if(pid ==0){  // 子进程执行cgi脚本
		char meth_env[255];
		char query_env[255];
		char length_env[255]; 
	}
}




























