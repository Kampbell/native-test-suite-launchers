#ifndef _GRADLE_TESTING_SOCKET_MOCK_H_
#define _GRADLE_TESTING_SOCKET_MOCK_H_

#include <cstdint>

#include "gmock/gmock.h"

#include "gradle/internal/socket.h"


namespace gradle { namespace testing {

class SocketMock {
  public:
    typedef int(socket_t)(int socket_family, int socket_type, int protocol);
    typedef int(connect_t)(SOCKET socket, const struct sockaddr * addr, socklen_t addrlen);
    typedef int(close_t)(SOCKET socket);
    typedef int(recv_t)(SOCKET socket, char * buffer, int length, int flags);
    typedef int(send_t)(SOCKET socket, const char * buffer, int length, int flags);
    typedef int(cleanup_t)(void);
    typedef int(startup_t)(uint16_t, void *);

  public:
    SocketMock();
    virtual ~SocketMock();

    ::testing::MockFunction<socket_t>& socket();
    ::testing::MockFunction<connect_t>& connect();
    ::testing::MockFunction<close_t>& close();
    ::testing::MockFunction<recv_t>& recv();
    ::testing::MockFunction<send_t>& send();
    ::testing::MockFunction<cleanup_t>& cleanup();
    ::testing::MockFunction<startup_t>& startup();

   private:
    ::testing::MockFunction<socket_t> socket_;
    ::testing::MockFunction<connect_t> connect_;
    ::testing::MockFunction<close_t> close_;
    ::testing::MockFunction<recv_t> recv_;
    ::testing::MockFunction<send_t> send_;
    ::testing::MockFunction<cleanup_t> cleanup_;
    ::testing::MockFunction<startup_t> startup_;
};

}  // namespace testing
}  // namespace gradle

#endif  // _GRADLE_TESTING_SOCKET_MOCK_H_
