#include <functional>
#include "gmock/gmock.h"
#include "WS2tcpip.h"
using namespace testing;

class WinsockMock;

WinsockMock* g_winsock_mock;

class WinsockMock {
  public:
    typedef int(WSACleanup_t)(void);
    typedef int(WSAStartup_t)(WORD, LPWSADATA);
    typedef int(socket_t)(int socket_family, int socket_type, int protocol);
    typedef int(connect_t)(SOCKET socket, const struct sockaddr * addr, socklen_t addrlen);
    typedef int(close_t)(SOCKET socket);
    typedef int(recv_t)(SOCKET socket, char * buffer, int length, int flags);
    typedef int(send_t)(SOCKET socket, const char * buffer, int length, int flags);

  public:
    WinsockMock() {
        if (nullptr != g_winsock_mock) {
            throw std::exception();
        }
        g_winsock_mock = this;
    }

    ~WinsockMock() {
        g_winsock_mock = nullptr;
    }

    MockFunction<socket_t>& socket() {
        return socket_;
    }
    MockFunction<connect_t>& connect() {
        return connect_;
    }
    MockFunction<close_t>& close() {
        return close_;
    }
    MockFunction<recv_t>& recv() {
        return recv_;
    }
    MockFunction<send_t>& send() {
        return send_;
    }
    MockFunction<WSACleanup_t>& wsacleanup() {
        return wsacleanup_;
    }
    MockFunction<WSAStartup_t>& wsastartup() {
        return wsastartup_;
    }

   private:
    MockFunction<socket_t> socket_;
    MockFunction<connect_t> connect_;
    MockFunction<close_t> close_;
    MockFunction<recv_t> recv_;
    MockFunction<send_t> send_;
    MockFunction<WSACleanup_t> wsacleanup_;
    MockFunction<WSAStartup_t> wsastartup_;
};


extern "C" int WSAAPI WSACleanup(void) {
    return g_winsock_mock->wsacleanup().Call();
}

extern "C" int
WSAAPI
WSAStartup(
     WORD wVersionRequested,
     LPWSADATA lpWSAData
    ) {
    return g_winsock_mock->wsastartup().Call(wVersionRequested, lpWSAData);
}



extern "C" SOCKET WSAAPI socket(int a1, int a2, int a3) {
    return g_winsock_mock->socket().Call(a1, a2, a3);
}


//int connect_wrapper(int a1, const struct sockaddr * a2, socklen_t a3);
extern "C" int WSAAPI connect(
    SOCKET a1,
    const struct sockaddr FAR * a2,
    int a3
    )
{
    return g_winsock_mock->connect().Call(a1, a2, a3);
}



extern "C" int WSAAPI closesocket(SOCKET a1) {
    return g_winsock_mock->close().Call(a1);
}


extern "C" int WSAAPI recv(SOCKET a1, char * a2, int a3, int a4) {
    return g_winsock_mock->recv().Call(a1, a2, a3, a4);
}


extern "C" int WSAAPI send(SOCKET a1, const char * a2, int a3, int a4) {
    return g_winsock_mock->send().Call(a1, a2, a3, a4);
}
