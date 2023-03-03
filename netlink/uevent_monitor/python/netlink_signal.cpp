#include <signal.h>
#include <stdlib.h>

#include <string>
#include <iostream>
#include "wrapsock.hpp"

#include <pybind11/pybind11.h>
namespace py = pybind11;

static void sig_int(int signo)
{
    pid_t pid;
    int status;

    std::cerr << signo << " recived\n";
    exit(signo);
    return;                     /* interrupts pause() */
}

class NetlinkUeventMonitor {
private:
    Socket sock;

public:
    NetlinkUeventMonitor(uint32_t pid): sock(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT) {
        sock.Bind(SocketAddress(NETLINK_KOBJECT_UEVENT, pid));
        signal(SIGINT, &sig_int);
        std::cerr << "NetlinkUeventMonitor construct\n";
    }

    std::string recv(int buf_size) {
        std::cerr << "NetlinkUeventMonitor recv\n";
        auto data = sock.Recv(buf_size);
        auto len = data.size();
        for(int i=0;i<len;i++) {
            if(data[i]=='\0')
                data[i]='\n';
            else if(!isprint(data[i]))
                data[i] = ' ';
        }
        return data;
    }
};

PYBIND11_MODULE(netlink, m) {
    py::class_<NetlinkUeventMonitor>(m, "NetlinkUeventMonitor")
        .def(py::init<uint32_t>())
        .def("recv", &NetlinkUeventMonitor::recv);
}
