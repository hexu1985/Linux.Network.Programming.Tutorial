#include "socket_istream.hpp"

using std::char_traits;
using std::basic_streambuf;
using std::istream;

template <class CharT, class Traits = char_traits<CharT> >
class SocketInputStreamBuffer : public basic_streambuf<CharT, Traits> {
public:
    typedef typename Traits::int_type                 int_type;

    SocketInputStreamBuffer(Socket* sock_): sock(sock_) 
    {
        this->setg(buffer, buffer + sizeof(buffer),
                buffer + sizeof(buffer));
    }

protected:
    int_type underflow() 
    {
        int len = sock->Recv(buffer, sizeof(buffer) * sizeof(CharT));

        if (len == 0) {
            return Traits::eof();
        }

        this->setg(buffer, buffer, buffer + len / sizeof(CharT));
        return this->sgetc();
    }

private:
    Socket *sock;
    CharT buffer[1024];
};

SocketInputStream::SocketInputStream(Socket& sock): istream(new SocketInputStreamBuffer<char>(&sock)) {
}

SocketInputStream::~SocketInputStream() {
    auto sb = static_cast<SocketInputStreamBuffer<char>*>(rdbuf());
    delete sb;
}
