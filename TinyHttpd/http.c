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
		
		
		/*
		
		子进程和父进程共用两个管道 
		
		子进程管道
			流程如主页图：括号中的最初端口被关闭
		(1) 父进程 只需要向管道写入数据和读取数据
		---------------------------------------------------------------
		 get/post数据 ==> cgi_input[1]----(cgi_input[0])
						 ...
						  (cgi_output[1])--cgi_output[0] ==> send浏览器
						  
		----------------------------------------------------------------
		(2) 子进程 处理数据		 
		 
					 (cgi_input[1])   ----     cgi_input[0]
				 ==>  stdin         -------      stdout
				 ==>  cgi_output[1] -------    (cgi_output[0])
		----------------------------------------------------------------			 
		*/
		// dup2 标准输出 重定向到将管道的写入端
		dup2(cgi_output[1],1 );
		// dup2 将标准输入 重定向 到管道的读取端
		dup2(cgi_input[0],0);
		
		// 关闭管道的原来端口
		close(cgi_output[0]);
		close(cgi_input[1]);
		
		
		//get 环境变量
		sprintf(meth_env, "REUEST_MTHEOD=%s",method);
		putenv(meth_env);
		if(strcasecmp(method,"GET")== 0){
			sprintf(query_env,"QUERY_STRING= %s", query_string);
			putenv(query_env);
		}
		else{
			// post
			sprintf(length_env, "CONTENT_LENGTH=%d",content_length);
			putenv(length_env);
		}
		
		// execl
		execl(path, path, NULL);
		exit(0);
	}
	else{  // 父进程
		close(cgi_output[1]);
		close(cgi_input[0]);
		
		// post
		if(strcasecmp(method, "POST") == 0){
			for(i=0;i<content_length;i++)  // 读取body
			{
				recv(client,&c, 1);
				write(cgi_input[1],&c, 1);  // 读取一个写一个
			}
		}
		
		// 从cgi_output 管道读取子进程的输出，并发送到客户端
		while(read(cgi_output[0],&c,1)>0)
			send(client,&c,0);
		
		
		// 关闭管道
		close(cgi_input[1]);
		clost(cgi_output[0]);
		
		// 等待子进程结束
		waitpid(pid,&status,0);
	}

}




/* get_line : 读取一行数据 */ 
int get_line(int sock, char* buf ,int size ){
	int i=0;
	char c='\0';
	int n;
	while((i<size -1) &&(c !='\n')){
		// recv 一个字节
		n = recv(sock, &c, 1,0);
		if(n>0){
			if(c=='\r'){ // 回车符
				// 读取一个字符，但是此字符在套接字缓冲区不移除
				n = recv(sock,&c,1,MSG_PEEK); // 读取下一个字符
				if((n>0) &&(c == '\n'))
					recv(sock,&c,1,0);
				else
					// c 不是 \n, 置 c为\n，终止
					c = '\n';
			}
			buf[i] = c;
			i++;
		}else{ // 读取失败，置c为‘\n’,退出循环
			c = '\n';
		}
		
	}
	buf[i] = '\0';
	return i;
}


/* headers : 加入http 的头部信息 */
void headers(int client, const char* filename){
	char buf[1024];
	(void)filename;  /* could use filename to determine file type */

	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(client, buf, strlen(buf), 0);	
}





//如果资源没有找到得返回给客户端下面的信息
void not_found(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "your request because the resource specified\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "is unavailable or nonexistent.\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(client, buf, strlen(buf), 0);
}



//如果不是CGI文件，直接读取文件返回给请求的http客户端
void serve_file(int client, const char *filename)
{
	FILE *resource = NULL;
	int numchars = 1;
	char buf[1024];

	//默认字符
	buf[0] = 'A'; buf[1] = '\0';
	while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
	numchars = get_line(client, buf, sizeof(buf));

	resource = fopen(filename, "r");
	if (resource == NULL)  // 文件不存
		not_found(client);
	else
	{  // 发送头部和文件内容
		headers(client, filename);
		cat(client, resource);
	}
	fclose(resource);  // 关闭文件
}



int startup(u_short *port)
{
	int httpd = 0;
	struct sockaddr_in name;

	httpd = socket(PF_INET, SOCK_STREAM, 0);
	if (httpd == -1)
		error_die("socket");  // socket建立失败
	
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(*port);
	name.sin_addr.s_addr = htonl(INADDR_ANY); // 地址建立
	
	//绑定socket
	if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
		error_die("bind");
	
	//如果端口没有设置，提供个随机端口
	if (*port == 0)  /* if dynamically allocating a port */
	{
		socklen_t  namelen = sizeof(name);
		if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
		error_die("getsockname");
		*port = ntohs(name.sin_port);
	}
	//监听
	if (listen(httpd, 5) < 0)
	error_die("listen");
	return(httpd);
}


//如果方法没有实现，就返回此信息
void unimplemented(int client)
{
 char buf[1024];

	sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</TITLE></HEAD>\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(client, buf, strlen(buf), 0);
}




int main(void)
{
	int server_sock = -1;
	u_short port = 0;
	int client_sock = -1;
	struct sockaddr_in client_name;

	//这边要为socklen_t类型
	socklen_t client_name_len = sizeof(client_name);
	pthread_t newthread;

	server_sock = startup(&port);
	printf("httpd running on port %d\n", port);

	while (1)
	{
	 //接受请求，函数原型
	 //#include <sys/types.h>
	 //#include <sys/socket.h>  
	 //int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	client_sock = accept(server_sock,
					   (struct sockaddr *)&client_name,
					   &client_name_len);
	if (client_sock == -1)
	  error_die("accept");
	/* accept_request(client_sock); */

	//每次收到请求，创建一个线程来处理接受到的请求
	//把client_sock转成地址作为参数传入pthread_create
	if (pthread_create(&newthread, NULL, (void *)accept_request, (void *)(intptr_t)client_sock) != 0)
	  perror("pthread_create");
	}

	close(server_sock);

	return(0);
}








