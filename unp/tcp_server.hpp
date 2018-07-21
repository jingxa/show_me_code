/**
	tcp server
*/

/* header */
#include "unp.h"







/**********************************/
/************* main ***************  */
int main(int argc ,char** argv){
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	
	struct sockaddr_in cliaddr ,servaddr;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	
	Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));
	
	Listen(listenfd, LISTENQ);
	for(;;){
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA*)&cliaddr, &clilen);
		
		if((childpid = Fork()) == 0){  // 子进程
			Close(listenfd);   		// 关闭监听套接字
			str_echo(connfd);		// 处理请求
			exit(0);
		}
		Close(connfd);  // 父进程关闭连接的connfd
	}
	
}

/**************  str_echo  函数 *********************/
void str_echo(int sockfd){
	ssize_t n;
	char buf[MAXLINE];
again:
	while((n =read(sockfd, buf ,MAXLINE)) >0 )
		Writen(sockfd, buf, n);
	
	if(n< 0 && errno == EINTER)
		goto again;
	
	else if(n < 0)
		err_sys("str_echo: read error");
}


/************** tcp 客户端 main *************/
int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	if(argc != 2)
		err_quit("usage: tcpcli <IPaddress>");
	
	sockfd = Socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT	);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr));
	Connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
	
	str_cli(stdin, sockfd);
	exit(0);
}



/******************* str_cli *************************/
void str_cli(FILE* fp, int sockfd){
	char sendlien[MAXLINE], recvline[MAXLINE];
	while(Fgets(sendfile, MAXLINE,fp) != NULL){
		if(Readline(sockfd, recvline, MAXLINE) == 0)
			err_quit("str_cli: server terminated prematurely");
		Fputs(recvline, stdout);
	}
}







/*******	SIGCHLD 信号 处理  *******/
// 例子：
signal(SIGCHLD, sighandler);
void sighandler(int signo){
	pid_t pid;
	int stat;
	pid = wait(&stat);
	printf("clild %sd terminated\n", pid);
	return;
}







/*************** tcp服务端 最终版本 *************************/
//  当fork子进程的时候，必须补货sigchld信号；
// 捕获信号的时候， 必须处理被中断的系统调用;
// SIGCHLD 的信号处理函数使用waitpid 函数以免留下僵尸进程；

int main(int argc ,char** argv){
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	
	void sig_chld(int);   // SIGCHLD处理函数
	
	struct sockaddr_in cliaddr ,servaddr;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	
	Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));
	
	Listen(listenfd, LISTENQ);
	Signal(SIGCHLD, sig_chld);   // 包装函数Singal中必须使用waitpid,使用参数WNOHANG，防止阻塞
	for(;;){
		clilen = sizeof(cliaddr);
		if(connfd = Accept(listenfd, (SA*)&cliaddr, &clilen)) < 0){
			if(error == EINTER)  // 系统调用中断
				continue;
			else
				err_sys("accept error");
		}
		
		if((childpid = Fork()) == 0){  // 子进程
			Close(listenfd);   		// 关闭监听套接字
			str_echo(connfd);		// 处理请求
			exit(0);
		}
		Close(connfd);  // 父进程关闭连接的connfd
	}
	
}



/*************** SIGPIPE 信号*******************/
/*
	当一个进程向某个接收到rst的套接字执行写操作，内核向进程发送SIGPIPE信号；，终止进程；
*/























