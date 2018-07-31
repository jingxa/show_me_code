#include <iostream>
#include<cassert>

#include<sys/socket.h>  //socket
#include<arpa/inet.h>   // 主机和网络服务转换
#include<sys/types.h>

#include<unistd.h>
#include<signal.h>
#include<errno.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/epoll.h>


#include "http_parser.h"
#include "http_process.h"
#include "threadpool.h"

#define MAX_EVENTS 10000

// 将描述符fd设置为非阻塞
int setnonblocking(const int fd){
    int old_options = fcntl(fd, F_GETFL);   // 获取fd的状态
    fcntl(fd,F_SETFL, old_options | O_NONBLOCK);  // 非阻塞方式
    return old_options;
}


/**
    将套接字的sockfd添加到epoll的监听列表
    is_one_shot用于选择是否开启EPOLLONESHOT选项
*/
void add_sockfd(const int epollfd, const int sockfd, const bool is_one_shot){
    epoll_event event;
    event.data.fd = sockfd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP; // 对fd感兴趣的集合
    if(is_one_shot){
        event.events |= EPOLLONESHOT;  // 在完成事件通知后禁用检查
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD, sockfd,&event);  // 添加fd
    setnonblocking(sockfd);
}


// 将sockfd从列表中移除
void rm_sockfd(const int epollfd, const int sockfd){
    epoll_ctl(epollfd, EPOLL_CTL_DEL,sockfd,0);
    close(sockfd);
}

// 改变sockfd上监听的事件
void modfd(const int epollfd, const int sockfd, const int ev){

    epoll_event event;
    event.data.fd = sockfd;
    event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epollfd,EPOLL_CTL_MOD, sockfd, &event);
}


// 注册信号signo的信号处理函数
void addsig(const int signo, void(handler)(int), bool is_restart = true){
    struct sigaction sa;
    memset(&sa, 0 , sizeof(sa));
    if(is_restart){
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);  // 初始化信号集拥有全部信号
    assert(sigaction(signo,&sa,NULL) != -1);
}


// 输出并向客户发送错误消息
void show_and_send_error(const int connfd, const std::string msg){
    std::cout<<"error:"<<msg<<std::endl;
    send(connfd, msg.c_str(), msg.size(),0);
    close(connfd);
}


int main(int argc ,char ** argv){

    if(argc != 2)
    {
        std::cout<<"用法:"<<argv[0]<<" <port>"<<std::endl;
        return 0;
    }

    int port = atoi(argv[1]);
    addsig(SIGPIPE,SIG_IGN);  // 忽略SIGPIPE信号

    threadpool<http_process> *pool;
    try{
        pool = new threadpool<http_process>();
    }catch(std::runtime_error e){
        std::cout<<"pool error:"<<e.what() <<std::endl;
        delete pool;
        return -1;
    }

    int listenfd = socket(AF_INET,SOCK_STREAM, 0);  // 创建一个socket
    assert(listenfd > 0);

    int optval = 1;
    int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,  // 套接字选项
                         &optval,sizeof(optval));
    assert(ret >=0);


    struct sockaddr_in servaddr;  // inet ipv4套接字地址
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;  // ipv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // ip
    servaddr.sin_port = htons(port);  // 端口

    // 强制转换ipv4套接字为通用套接字，绑定到listenfd
    ret = bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    assert(ret >= 0);

    // 监听listenfd,未决的连接请求最大为5
    ret = listen(listenfd, 5);
    assert(ret >=0);


    // epoll
    struct epoll_event evlist[MAX_EVENTS];
    int epollfd = epoll_create(5);

    assert(epollfd != -1);
    add_sockfd(epollfd,listenfd,false);  // 添加listenfd到epoll兴趣列表

    //
    while(true){
        // 返回epoll就绪列表中的描述符数量
        int ready  = epoll_wait(epollfd,evlist,MAX_EVENTS, -1);
        if(ready < 0 && errno != EINTR){
            std::cout<<"epoll_wait 失败！"<<std::endl;
            break;
        }

        for(int i =0;i <ready;++i){
            int sockfd = evlist[i].data.fd;  //
            // 有新连接的到来
            if(sockfd == listenfd){
                struct sockaddr_in clientaddr;
                socklen_t clientaddr_len = sizeof(clientaddr);
                int connfd = accept(listenfd,(struct sockaddr*)&clientaddr,
                                    &clientaddr_len);
                if(connfd < 0){
                    std::cout<<"error: "<<strerror(errno)<<std::endl;
                    continue;
                }
                add_sockfd(epollfd,connfd,true); //

             // epoll 出错
            }else if(evlist[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
                close(sockfd);

            }

            // 有新的请求到来
            else if(evlist[i].events & EPOLLIN)
            {
                pool->add(new http_process(epollfd, sockfd));
            }
        }

    }



    close(listenfd);
    close(epollfd);
    delete pool;
    return 0;





}



























