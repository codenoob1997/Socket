#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#ifndef CONNECT_SIZE
#define CONNECT_SIZE 256
#endif

#define PORT 8888
#define MAX_LINE 2048
#define LISTENQ 20


//将socket改变为非阻塞状态
void setNonblocking(int sockfd){
    int opts;
    opts = fcntl(sockfd,F_GETFL);
    if(opts<0){                         
        perror("fcntol(sock,GETFL)");
        return;
    }//if
    opts = opts|O_NONBLOCK; //设置为阻塞状态  00000020(可读写) | 00004000(NONBLOCK) -->  00004020
    if(fcntl(sockfd,F_SETFL,opts)<0){       //设置失败
        printf("fcntl(sock,SETFL,opts)");
        return ;
    }//if
}

int main(int argc,char **argv){
    int i,listenfd,connfd,sockfd,epfd,nfds;   //nfds = number of file descriptors 
    ssize_t n , ret;
    char buf[MAX_LINE];  // Message Buffer

    socklen_t clilen ; 

    struct sockaddr_in servaddr , cliaddr ;

    /*epoll结构体  ev用于注册   数组用于回传*/
    struct epoll_event ev,events[20];

    listenfd = socket(AF_INET,SOCK_STREAM,0);
    setNonblocking(listenfd); //listenfd默认阻塞

    /*生成用於處理accept的epoll專用文件描述符*/
    epfd = epoll_create(CONNECT_SIZE);
    /*设置监听描述符*/
    ev.data.fd = listenfd;
    /*設置處理事件類型*/
    ev.events = EPOLLIN | EPOLLET;
    /*注册事件*/
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

    /*Bind*/
    bzero(&servaddr,sizeof(servaddr));//memset()
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDDR_ANY = 0

    bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    /*Listen*/
    listen(listenfd,LISTENQ);


    /*RECV LOOP*/
    while(1){
        /*等待事件发生*/
        nfds = epoll_wait(epfd,events,CONNECT_SIZE,-1);
        if(nfds<=0)
            continue;

        /*处理事件*/
        for(i = 0; i<nfds;++i){
            if(events[i].data.fd == listenfd) /*如果是listenfd发生事件就是连接请求*/
            {
                    clilen = sizeof(cliaddr);

                    if((connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen))<0)
                    {
                            {
                                printf("accept  error : %s, errno ",strerror(errno),errno);
                                exit(1);
                            }
                    }//if
                    printf("accept a new client : %s :%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);

                /*将其设置为非阻塞*/
                setNonblocking(connfd);
                ev.data.fd = connfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);

            }//if
            else if(events[i].events & EPOLLIN){
                if((sockfd = events[i].data.fd )<0) continue;

                bzero(buf,MAX_LINE);
                printf("reading socket ~~~~~\n");
                if((n = read(sockfd,buf,MAX_LINE))<=0){
                    close(sockfd);
                    events[i].data.fd = -1;
                }//if
                else{
                    buf[n] = '\0';
                    printf("client[%d] send message : %s \n",i,buf);
                }//else

            }//else
        }//for
    }//while
    free(events);
    close(epfd);
    exit(0);



}