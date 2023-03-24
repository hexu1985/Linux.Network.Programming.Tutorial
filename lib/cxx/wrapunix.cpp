#include "wrapunix.hpp"
#include "error.hpp"

#include <cassert>

std::string Gethostname(std::error_code& ec) {
    char host_name[128] = {0};
    if (gethostname(host_name, sizeof(host_name)) < 0) {
        ec.assign(errno, std::system_category());
        return "";
    }
    return host_name;
}

std::string Gethostname() {
    std::error_code ec;
    auto host_name = Gethostname(ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Gethostname() error");
    }
    return host_name;
}

namespace {

int MaxFd(const std::vector<int>& readfds, const std::vector<int>& writefds, const std::vector<int>& exceptfds) {
    std::vector<int> total;
    total.insert(total.end(), readfds.begin(), readfds.end());
    total.insert(total.end(), writefds.begin(), writefds.end());
    total.insert(total.end(), exceptfds.begin(), exceptfds.end());
    assert(total.size() != 0);
    return *std::max_element(total.begin(), total.end());
}

void FillFdSet(fd_set* fdsetp, const std::vector<int>& fdlist) {
    FD_ZERO(fdsetp);
    for (auto fd : fdlist) {
        FD_SET(fd, fdsetp);
    }
}

std::vector<int> TestFdSet(fd_set* fdsetp, const std::vector<int>& fdlist) {
    std::vector<int> reslist;
    for (auto fd : fdlist) {
        if (FD_ISSET(fd, fdsetp)) {
            reslist.push_back(fd);
        }
    }
    return reslist;
}

}   // namespace

std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>
Select(const std::vector<int>& readfds, const std::vector<int>& writefds, const std::vector<int>& exceptfds, 
        struct timeval* timeout, std::error_code& ec) {
    fd_set rset, wset, eset;
    FillFdSet(&rset, readfds);
    FillFdSet(&wset, writefds);
    FillFdSet(&eset, exceptfds);

    int nfds = MaxFd(readfds, writefds, exceptfds)+1;
    
    int n;
again:
    if ( (n = select(nfds, &rset, &wset, &eset, timeout)) < 0) {
        if (errno == EINTR)
            goto again;
        else {
            ec.assign(errno, std::system_category());
            return {};
        }
    } 
    if (n == 0) {
        if (timeout == nullptr) {
            ec.assign(ECONNABORTED, std::system_category());
        } else {
            ec.assign(ETIMEDOUT, std::system_category());
        }
        return {};
    }

    return std::make_tuple(TestFdSet(&rset, readfds), TestFdSet(&wset, writefds), TestFdSet(&eset, exceptfds));
}

std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>
Select(const std::vector<int>& readfds, const std::vector<int>& writefds, const std::vector<int>& exceptfds, 
      std::error_code& ec) {
    return Select(readfds, writefds, exceptfds, nullptr, ec);
}

std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>
Select(const std::vector<int>& readfds, const std::vector<int>& writefds, const std::vector<int>& exceptfds, 
        struct timeval* timeout) {
    std::error_code ec;
    auto ret = Select(readfds, writefds, exceptfds, timeout, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Select() error");
    }
    return ret;
}
