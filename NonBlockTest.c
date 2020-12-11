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

#define MAX_LINE  2048
#define PORT  8888
#define BACKLOG  10
#define LISTENQ  6666
#define MAX_CONNECT  20

int main(){
    int flag;
    int sockfd;
    sockfd = socket(AF_INET,SOCK_STREAM,0);     
    flag = fcntl(sockfd,F_GETFL);       //拿他的FLAG
    printf("flag -- %x",flag);          //00000020 读写都可O_RDWR
    flag = flag| O_NONBLOCK;            //  00000020 | 00004000  
    printf("flag -- %x",flag);          //00004020  //   O_RDWR  | O_NONBLOCK 

}