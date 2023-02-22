#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>

#include "wrapsock.hpp"

#define MAX_BYTES   4096

void MonitorNetlinkUevent()
{
    Socket sock(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
    sock.Bind(SocketAddress(NETLINK_KOBJECT_UEVENT, 0));

    int count = 0;
    for (;;) {
        auto data = sock.Recv(MAX_BYTES);
        auto len = data.length();
        if (len < 32) {
            std::cout << "invalid message" << std::endl;
            continue;
        }

        for(int i=0;i<len;i++) {
            if(data[i]=='\0')
                data[i]='\n';
            else if(!isprint(data[i]))
                data[i] = '.';
        }

        count++;
        std::cout << "***********************msg " << count << " start***********************\n";
        std::cout << "received " << len << " bytes\n" << data << "\n";
        std::cout << "***********************msg " << count << " ends************************\n";
        std::cout << std::flush;
    }
}

int main(int argc,char **argv)
{
    std::cout << "***********************start***********************" << std::endl;
    MonitorNetlinkUevent();
    std::cout << "***********************ends************************" << std::endl;

    return 0;
}
