#include <string>
#include "wrapsock.hpp"

#include <pybind11/pybind11.h>
namespace py = pybind11;

class NetlinkUeventMonitor {
private:
    Socket sock;

public:
    NetlinkUeventMonitor(uint32_t pid): sock(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT) {
        sock.Bind(SocketAddress(NETLINK_KOBJECT_UEVENT, pid));
    }

    std::string recv(int buf_size) {
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
