#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "channel.h"
#include "internal/channel-impl.h"

using namespace testing;

std::function<socket_t> mock_socket_wrapper;
int socket_wrapper(int a1, int a2, int a3) {
    return mock_socket_wrapper(a1, a2, a3);
}

std::function<connect_t> mock_connect_wrapper;
int connect_wrapper(int a1, const struct sockaddr * a2, socklen_t a3) {
    return mock_connect_wrapper(a1, a2, a3);
}

std::function<close_t> mock_close_wrapper;
int close_wrapper(int a1) {
    return mock_close_wrapper(a1);
}

std::function<recv_t> mock_recv_wrapper;
size_t recv_wrapper(int a1, void * a2, size_t a3, int a4) {
    return mock_recv_wrapper(a1, a2, a3, a4);
}

std::function<send_t> mock_send_wrapper;
size_t send_wrapper(int a1, void * a2, size_t a3, int a4) {
    return mock_send_wrapper(a1, a2, a3, a4);
}

#define VALID_SOCKET 1

class SocketChannelTest : public Test {
  public:
  void SetUp() {
    mock_socket_wrapper = mock_socket.AsStdFunction();
    mock_connect_wrapper = mock_connect.AsStdFunction();
    mock_close_wrapper = mock_close.AsStdFunction();
    mock_recv_wrapper = mock_recv.AsStdFunction();
    mock_send_wrapper = mock_send.AsStdFunction();

    // NOTE: VS doesn't implement list initalization for member variable
    config = {
      kSocketChannel,
      socket_wrapper,
      connect_wrapper,
      close_wrapper,
      recv_wrapper,
      send_wrapper
    };
  }
    MockFunction<socket_t> mock_socket;
    MockFunction<connect_t> mock_connect;
    MockFunction<close_t> mock_close;
    MockFunction<recv_t> mock_recv;
    MockFunction<send_t> mock_send;

    gradle_channel_configuration_t config;
};

TEST_F(SocketChannelTest, initialize_with_invalid_socket_function) {
    // Given:
    config.socket = NULL;
    EXPECT_THAT(config.socket, IsNull());

    // When:
    ASSERT_THAT(gradle_channel_initialize(&config), Eq(kInvalidSocketFunction));
}

TEST_F(SocketChannelTest, initialize_with_invalid_connect_function) {
    // Given:
    config.connect = NULL;
    EXPECT_THAT(config.connect, IsNull());

    // When:
    ASSERT_THAT(gradle_channel_initialize(&config), Eq(kInvalidConnectFunction));
}

TEST_F(SocketChannelTest, initialize_with_invalid_close_function) {
    // Given:
    config.close = NULL;
    EXPECT_THAT(config.close, IsNull());

    // When:
    ASSERT_THAT(gradle_channel_initialize(&config), Eq(kInvalidCloseFunction));
}

TEST_F(SocketChannelTest, initialize_with_invalid_receive_function) {
    // Given:
    config.recv = NULL;
    EXPECT_THAT(config.recv, IsNull());

    // When:
    ASSERT_THAT(gradle_channel_initialize(&config), Eq(kInvalidReceiveFunction));
}

TEST_F(SocketChannelTest, initialize_with_invalid_send_function) {
    // Given:
    config.send = NULL;
    EXPECT_THAT(config.send, IsNull());

    // When:
    ASSERT_THAT(gradle_channel_initialize(&config), Eq(kInvalidSendFunction));
}

TEST_F(SocketChannelTest, initialize_with_invalid_type) {
    // Given:
    config.type = 0;
    EXPECT_THAT(config.type, Ne(kSocketChannel));

    // When:
    ASSERT_THAT(gradle_channel_initialize(&config), Eq(kInvalidChannelType));
}

TEST_F(SocketChannelTest, socket_return_invalid_socket_during_initialize) {
    // Given:
    InSequence s;

    // Given:
    EXPECT_CALL(mock_socket, Call(_, _, _))
        .Times(1)
        .WillOnce(Return(INVALID_SOCKET));

    // TODO(daniel): Should expect call to WSACleanup on windows...

    // When:
    ASSERT_THAT(gradle_channel_initialize(&config), Eq(kCantCreateSocket));
}

TEST_F(SocketChannelTest, connect_return_error_during_initialize) {
    // Given:
    EXPECT_CALL(mock_socket, Call(_, _, _)).Times(AnyNumber());
    ON_CALL(mock_socket, Call(_, _, _)).WillByDefault(Return(VALID_SOCKET));

    // Given:
    InSequence s;

    // Given:
    EXPECT_CALL(mock_connect, Call(_, _, _))
        .Times(1)
        .WillOnce(Return(SOCKET_ERROR));

    // Given:
    EXPECT_CALL(mock_close, Call(Eq(VALID_SOCKET))).Times(1);

    // TODO(daniel): Should expect call to WSACleanup

    // When:
    ASSERT_THAT(gradle_channel_initialize(&config), Eq(kCantConnectClientToServer));

    // Then:
    ASSERT_THAT(socketfd, Eq(INVALID_SOCKET));
    ASSERT_THAT(g_context, IsNull());
}

TEST_F(SocketChannelTest, initialize_successfully) {
    // TODO(daniel): Should not expect call to WSACleanup
    // Given:
    EXPECT_CALL(mock_close, Call(_)).Times(0);

    // Given:
    InSequence s;

    // Given:
    EXPECT_CALL(mock_socket, Call(Eq(AF_INET), Eq(SOCK_STREAM), Eq(IPPROTO_TCP)))
        .Times(1)
        .WillOnce(Return(VALID_SOCKET));

    // Given:
    EXPECT_CALL(mock_connect, Call(Eq(VALID_SOCKET), NotNull(), Eq(sizeof(struct sockaddr_in))))
        .Times(1)
        .WillOnce(Return(0));

    // When:
    ASSERT_THAT(gradle_channel_initialize(&config), Eq(kSuccess));

    // Then:
    ASSERT_THAT(socketfd, Eq(VALID_SOCKET));
    ASSERT_THAT(g_context, NotNull());
}
