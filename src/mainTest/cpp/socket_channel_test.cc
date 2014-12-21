#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "channel.h"
#include "internal/channel-impl.h"

#include "gradle/testing/winsock_mock.h"

using namespace testing;


#define VALID_SOCKET 1

class SocketChannelTest : public Test {
  public:
  void SetUp() {
    EXPECT_THAT(socketfd, Eq(INVALID_SOCKET));

    // NOTE: VS doesn't implement list initalization for member variable
    config = {
      kSocketChannel
    };
  }

  void TearDown() {
    socketfd = INVALID_SOCKET;
  }
    WinsockMock winsock;

    gradle_channel_configuration_t config;
};

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
    EXPECT_CALL(winsock.socket(), Call(_, _, _))
        .Times(1)
        .WillOnce(Return(INVALID_SOCKET));
    EXPECT_CALL(winsock.wsacleanup(), Call()).Times(1);

    // When:
    ASSERT_THAT(gradle_channel_initialize(&config), Eq(kCantCreateSocket));
}

TEST_F(SocketChannelTest, connect_return_error_during_initialize) {
    // Given:
    EXPECT_CALL(winsock.socket(), Call(_, _, _)).Times(AnyNumber());
    ON_CALL(winsock.socket(), Call(_, _, _)).WillByDefault(Return(VALID_SOCKET));

    // Given:
    InSequence s;

    // Given:
    EXPECT_CALL(winsock.connect(), Call(_, _, _))
        .Times(1)
        .WillOnce(Return(SOCKET_ERROR));

    // Given:
    EXPECT_CALL(winsock.close(), Call(Eq(VALID_SOCKET))).Times(1);
    EXPECT_CALL(winsock.wsacleanup(), Call()).Times(1);

    // When:
    ASSERT_THAT(gradle_channel_initialize(&config), Eq(kCantConnectClientToServer));

    // Then:
    ASSERT_THAT(socketfd, Eq(INVALID_SOCKET));
    ASSERT_THAT(g_context, IsNull());
}

TEST_F(SocketChannelTest, initialize_successfully) {
    // Given:
    EXPECT_CALL(winsock.close(), Call(_)).Times(0);
    EXPECT_CALL(winsock.wsacleanup(), Call()).Times(0);

    // Given:
    InSequence s;

    // Given:
    EXPECT_CALL(winsock.socket(), Call(Eq(AF_INET), Eq(SOCK_STREAM), Eq(IPPROTO_TCP)))
        .Times(1)
        .WillOnce(Return(VALID_SOCKET));

    // Given:
    EXPECT_CALL(winsock.connect(), Call(Eq(VALID_SOCKET), NotNull(), Eq(sizeof(struct sockaddr_in))))
        .Times(1)
        .WillOnce(Return(0));

    // When:
    ASSERT_THAT(gradle_channel_initialize(&config), Eq(kSuccess));

    // Then:
    ASSERT_THAT(socketfd, Eq(VALID_SOCKET));
    ASSERT_THAT(g_context, NotNull());
}
