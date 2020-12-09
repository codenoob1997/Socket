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

int max(int a,int b){
    return a>b?a:b;
}

void str_cli(int sockfd){
    char sendline[MAX_LINE];
    while(fgets(sendline,MAX_LINE,stdin)!=NULL){
        write(sockfd,sendline,strlen(sendline)-1);
        bzero(sendline,MAX_LINE);
    }
}
int main(int argc , char **argv){
    int sockfd;
    struct sockaddr_in servaddr;
    if(argc!=2){
        printf("usage: ./client <ip address>");
        exit(1);
    }//if

    if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0){
        printf("cannot create socket error : %s  errno: %d\n",strerror(errno),errno);
        exit(1);
    }

    /*initiate socket information*/
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET,argv[1],&servaddr.sin_addr.s_addr);


    if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
        printf("cannot connect server ! error : %s   errno : %d\n",strerror(errno),errno);
        exit(1);
    }
    str_cli(sockfd);
    exit(0);
}