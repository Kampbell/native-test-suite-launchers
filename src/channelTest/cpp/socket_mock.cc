#include <exception>

#include "gradle/testing/socket_mock.h"

using ::testing::MockFunction;

namespace gradle { namespace testing {

static SocketMock* g_mock;

SocketMock::SocketMock() {
    if (nullptr != g_mock) {
        throw std::exception();
    }
    g_mock = this;
}

SocketMock::~SocketMock() {
    g_mock = nullptr;
}

MockFunction<SocketMock::socket_t>& SocketMock::socket() {
    return socket_;
}
MockFunction<SocketMock::connect_t>& SocketMock::connect() {
    return connect_;
}
MockFunction<SocketMock::close_t>& SocketMock::close() {
    return close_;
}
MockFunction<SocketMock::recv_t>& SocketMock::recv() {
    return recv_;
}
MockFunction<SocketMock::send_t>& SocketMock::send() {
    return send_;
}

MockFunction<SocketMock::cleanup_t>& SocketMock::cleanup() {
    return cleanup_;
}
MockFunction<SocketMock::startup_t>& SocketMock::startup() {
    return startup_;
}

}  // namespace testing
}  // namespace gradle


extern "C" SOCKET WSAAPI socket(int a1, int a2, int a3) {
    return ::gradle::testing::g_mock->socket().Call(a1, a2, a3);
}


//int connect_wrapper(int a1, const struct sockaddr * a2, socklen_t a3);
extern "C" int WSAAPI connect(
    SOCKET a1,
    const struct sockaddr FAR * a2,
    int a3
    )
{
    return ::gradle::testing::g_mock->connect().Call(a1, a2, a3);
}



extern "C" int WSAAPI closesocket(SOCKET a1) {
    return ::gradle::testing::g_mock->close().Call(a1);
}


extern "C" int WSAAPI recv(SOCKET a1, char * a2, int a3, int a4) {
    return ::gradle::testing::g_mock->recv().Call(a1, a2, a3, a4);
}


extern "C" int WSAAPI send(SOCKET a1, const char * a2, int a3, int a4) {
    return ::gradle::testing::g_mock->send().Call(a1, a2, a3, a4);
}

extern "C" int WSAAPI WSACleanup(void) {
    return ::gradle::testing::g_mock->cleanup().Call();
}

extern "C" int
WSAAPI
WSAStartup(
     uint16_t wVersionRequested,
     WSADATA * lpWSAData
    ) {
    return ::gradle::testing::g_mock->startup().Call(wVersionRequested, lpWSAData);
}
