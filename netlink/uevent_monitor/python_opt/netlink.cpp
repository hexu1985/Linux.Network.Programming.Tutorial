#include <string>
#include <set>
#include <thread>
#include "wrapsock.hpp"

#include <pybind11/pybind11.h>
namespace py = pybind11;

class UeventObserver {
public:
    virtual ~UeventObserver() { }
    virtual void onReceiveUevent(const std::string& data) = 0;
};

class PyUeventObserver : public UeventObserver {
public:
    using UeventObserver::UeventObserver;

    void onReceiveUevent(const std::string& data) override {
        PYBIND11_OVERRIDE_PURE(
                void,
                UeventObserver,
                onReceiveUevent,
                data
                );
    }
    
};

class NetlinkUeventMonitor {
private:
    Socket sock;
    std::set<UeventObserver*> observer_list;
    std::thread sock_thread;

public:
    NetlinkUeventMonitor(uint32_t pid): sock(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT) {
        sock.Bind(SocketAddress(NETLINK_KOBJECT_UEVENT, pid));
    }

    void addObserver(UeventObserver* observer) {
        observer_list.insert(observer);
    }

    void deleteObserver(UeventObserver* observer) {
        observer_list.erase(observer);
    }

    void notifyObservers(const std::string& data) {
        for (auto observer : observer_list) {
            observer->onReceiveUevent(data);
        }
    }

    void start() {
        sock_thread = std::thread(&NetlinkUeventMonitor::run, this);
    }

    void run() {
        while (true) {
            auto data = recv(4096);
            notifyObservers(data);
        }
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
    py::class_<UeventObserver, PyUeventObserver>(m, "UeventObserver")
        .def(py::init<>())
        .def("onReceiveUevent", &UeventObserver::onReceiveUevent);

    py::class_<NetlinkUeventMonitor>(m, "NetlinkUeventMonitor")
        .def(py::init<uint32_t>())
        .def("addObserver", &NetlinkUeventMonitor::addObserver)
        .def("deleteObserver", &NetlinkUeventMonitor::deleteObserver)
        .def("start", &NetlinkUeventMonitor::start);
}
