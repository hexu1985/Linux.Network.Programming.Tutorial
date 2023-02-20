#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>

#include "wrapsock.h"

#define MAX_BYTES   4096

void MonitorNetlinkUevent()
{
    int sockfd;
    struct sockaddr_nl sa;
    int len;
    char buf[MAX_BYTES];
    int i;

    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_groups = NETLINK_KOBJECT_UEVENT;
    sa.nl_pid = 0;//getpid(); both is ok

    sockfd = Socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);

    Bind(sockfd, (struct sockaddr *) &sa, sizeof(sa));

    int count = 0;
    for (;;) {
        memset(buf, 0, sizeof(buf));
        len = Recv(sockfd, buf, MAX_BYTES, 0);
        if(len < 32) {
            printf("invalid message");
            continue;
        }

        count++;
        printf("***********************msg %d start***********************\n", count);
        for(i=0;i<len;i++) {
            if(*(buf+i)=='\0')
                buf[i]='\n';
            else if(!isprint(*(buf+i)))
                buf[i] = '.';
        }
        printf("received %d bytes\n%s\n",len,buf);
        printf("***********************msg %d ends************************\n", count);
        fflush(stdout);
    }
    Close(sockfd);
}

int main(int argc,char **argv)
{
    printf("***********************start***********************\n");
    fflush(stdout);
    MonitorNetlinkUevent();
    printf("***********************ends************************\n");
    fflush(stdout);
    return 0;
}
