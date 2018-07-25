#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>


int main(int argc, char **argv){
	int sockfd;
	int len;
	struct sockaddr_in address;  // ipv4
	int result ;
	char ch = 'A';
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);  //tcp
	address.sin_family  =AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");    // ip char*  to in_addr_t
	address.sin_port = htons(9734);  // port
	
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr*)&address, len);   // 客户端链接服务器
	
	if(result == -1){
		perror("oops: client1");
		exit(1);
		
	}
	
	
	write(sockfd, &ch, 1);
	read(sockfd, &ch,1);
	printf("char from server = %c\n ",ch);
	close(sockfd);
	exit(0);

	
}