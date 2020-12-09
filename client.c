#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#define PORT 8888
#define MAX_LINE 2048

int main(int argv,char **argc){
    struct sockaddr_in servaddr;
    int sockfd,connfd;
    if(argv<2){
        printf("usage : ./client <ipaddress>");
        exit(0);
    }

    //新建socket
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0){
        printf("cannot create socket ! error : %s  errno: %d\n",strerror(errno),errno);
        exit(0);
    }

    //初始化socket
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET,argc[1],&servaddr.sin_addr.s_addr);

    //connct
    if((connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)){
        printf("cannot connect to server ! error : %s   errno:%d\n",strerror(errno),errno);
        exit(0);
    }
    printf("conncted server!");

    char sendline[MAX_LINE];
    while(fgets(sendline,MAX_LINE,stdin)!=NULL){
        write(sockfd,sendline,strlen(sendline)-1);
    }
    close(sockfd);


}