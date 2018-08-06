#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "rio.h"


// fd 中 读取 n 个字节 到 usrbuf
ssize_t rio_readn(int fd, void *usrbuf, size_t n){
	size_t nleft = n;
	ssize_t nread;
	char *bufp = (char*)usrbuf;

	while(nleft >0){
		if((nread = read(fd, bufp, nleft)) < 0){  
			if( EINTR == errno)   // 遇到中断，重读
				nread = 0;
			else
				return -1;
		}
		else if( 0 == nread)  // 读到换行符， 结束
			break;
		nleft -= nread;
		bufp += nread;
	}

	return n - nleft;  // 读取成功的字符
}


// 向fd中写入 n个字节
ssize_t rio_writen(int fd, void *usrbuf, size_t n){

	size_t nleft = n;
	ssize_t nwrittern;
	char *bufp = (char*)usrbuf;

	while(nleft > 0){
		if((nwrittern = write(fd, bufp, nleft)) < 0){
			if( EINTR == errno)
				nwrittern = 0;
			else{
				return -1;
			}
		}
		nleft -= nwrittern;
		bufp += nwrittern;
	}
	return n;
}

// 从 rio_t 的缓冲区中 读取 n个字节写入到 usrbuf 中
static ssize_t rio_read(rio_t *rp, char *usrbuf,size_t n){
	size_t cnt; 
	while(rp->rio_cnt <=0){
		rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
		if(rp->rio_cnt < 0){
			if( EAGAIN == errno){  // 文件阻塞
				return -EAGAIN;
			}
			if( EINTR == errno)   // 系统中断
			{
				return -1;
			}
		}else if(rp->rio_cnt == 0){  // 文件结束
			return 0;
		}else{
			rp->rio_bufptr = rp->rio_buf;  // 指针指向存储空间开头
		}
	}

	cnt = n;
	if(rp->rio_cnt < (ssize_t)n)
	{
		cnt = rp->rio_cnt;
	}
	memcpy(usrbuf, rp->rio_bufptr, cnt);
	rp->rio_bufptr += cnt;  // 指针指向空闲空间的开头
	rp->rio_cnt -= cnt;  // 清除缓冲区计算

	return cnt;
}


// 使用描述符fd 初始化一个 rio_t 结构体
void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}


// 从  rio_t 缓冲区 读取n个字节 到 usrbuf
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = (char *)usrbuf;
    while (nleft > 0){
        if((nread = rio_read(rp, bufp, nleft)) < 0){
            if(errno == EINTR)  // 中断 ，重读
                nread = 0;
            else
                return -1;
        }
        else if(nread == 0)  // eof 文件结尾
            break;
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);
}


// 从 rio_t 的缓冲区中读取一行 写入到 usrbuf中
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    size_t n;
    ssize_t rc;
    char c, *bufp = (char *)usrbuf;
    for(n = 1; n < maxlen; n++){
        if((rc = rio_read(rp, &c, 1)) == 1){
            *bufp++ = c;
            if(c == '\n')  // 一行
            break;
        }
        else if(rc == 0){  // eof 文件结尾
            if (n == 1){   // 一个字节都没读到
                return 0;
            }
            else
                break;
        }
        else if(rc == -EAGAIN){   // 文件阻塞
            return rc;
        }
        else{
            return -1;
        }
    }
    *bufp = 0;
    return n;    //返回读取字节数
}
