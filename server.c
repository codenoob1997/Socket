#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAX_LINE  2048
#define PORT  8888
#define BACKLOG  10
#define LISTENQ  6666
#define MAX_CONNECT  20

int main(int argv , char **argc){
    int sockfd,connfd;
    struct sockaddr_in servaddr;
    socklen_t clilen;
    if(argv<1){
        printf("usage : ./server <ipaddress>\n");
        exit(0);
    }

    //創建socket
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0){
        printf("cannot create socket ! error : %s  errno: %d",strerror(errno),errno);
        exit(0);
    }

    //初始化socket
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET,argc[0],&servaddr.sin_addr.s_addr);


    //bind()
    if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
        printf("cannot bind socket err : %s  errno : %d",strerror(errno),errno);
        exit(0);

    }

    //listen
    if(listen(sockfd,LISTENQ)<0){
        printf("listen error : %s  errno: %d",strerror(errno),errno);
        exit(0);
    }
    clilen = sizeof(servaddr);
    char recvline[MAX_LINE];
    if((connfd = accept(sockfd,(struct sockaddr*)&servaddr,&clilen))<0){
            printf("cannot connect err: %s  errno:  %d",strerror(errno),errno);
            exit(0);

        }
    read(connfd,recvline,MAX_LINE);
    close(sockfd);
    close(connfd);
}