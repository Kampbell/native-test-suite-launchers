#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "gradle/channel.h"
#include "gradle/internal/socket_channel.h"

#include "gradle/testing/socket_mock.h"

using namespace testing;


#define VALID_SOCKET (SOCKET)(1)

class SocketChannel : public Test {
  public:
      void SetUp() {
        EXPECT_THAT(socketfd, Eq(INVALID_SOCKET));

        // NOTE: VS doesn't implement list initalization for member variable
        config = {
          {kSocketChannel}, "127.0.0.1", 1337
        };
      }

      void TearDown() {
        socketfd = INVALID_SOCKET;
      }
    ::gradle::testing::SocketMock mock;

    gradle_socket_channel_configuration_t config;
    gradle_channel_configuration_t * const config_ref = &config.header;
};

class SocketChannel_Initialize : public SocketChannel {};

TEST_F(SocketChannel_Initialize, startup_return_error__cleanup_and_throw_kStartupFail) {
    // Given:
    InSequence s;
    EXPECT_CALL(mock.startup(), Call(_, _)).Times(1).WillOnce(Return(-1));
    EXPECT_CALL(mock.cleanup(), Call()).Times(1);

    // When:
    ASSERT_THAT(gradle_channel_initialize(config_ref), Eq(kStartupFail));
}

TEST_F(SocketChannel_Initialize, invalid_ipv4_address__cleanup_and_throw_kInvalidIpv4Address) {
    // Given:
    EXPECT_CALL(mock.startup(), Call(_, _)).Times(AnyNumber()).WillOnce(Return(NO_ERROR));
    EXPECT_CALL(mock.cleanup(), Call()).Times(3);

    const char * null_address = nullptr;
    const char * invalid_ipv4_address = "400.23.1.1";
    const char * ipv6_address = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";

    // When:
    config.address = null_address;
    ASSERT_THAT(gradle_channel_initialize(config_ref), Eq(kInvalidIpv4Address));

    config.address = invalid_ipv4_address;
    ASSERT_THAT(gradle_channel_initialize(config_ref), Eq(kInvalidIpv4Address));

    config.address = ipv6_address;
    ASSERT_THAT(gradle_channel_initialize(config_ref), Eq(kInvalidIpv4Address));
}

TEST_F(SocketChannel_Initialize, invalid_configuration_type__cleanup_and_throw_kInvalidChannelType) {
    // Given:
    config.header.type = 0;
    EXPECT_THAT(config.header.type, Ne(kSocketChannel));

    // Given:
    EXPECT_CALL(mock.startup(), Call(_, _)).Times(AnyNumber()).WillOnce(Return(NO_ERROR));
    EXPECT_CALL(mock.cleanup(), Call()).Times(1);

    // When:
    ASSERT_THAT(gradle_channel_initialize(config_ref), Eq(kInvalidChannelType));
}

TEST_F(SocketChannel_Initialize, socket_return_invalid_socket__cleanup_and_throw_kCantCreateSocket) {
    // Given:
    InSequence s;

    // Given:
    EXPECT_CALL(mock.startup(), Call(_, _)).Times(AnyNumber()).WillOnce(Return(NO_ERROR));
    EXPECT_CALL(mock.socket(), Call(_, _, _)).Times(1).WillOnce(Return(INVALID_SOCKET));
    EXPECT_CALL(mock.cleanup(), Call()).Times(1);

    // When:
    ASSERT_THAT(gradle_channel_initialize(config_ref), Eq(kCantCreateSocket));
}

TEST_F(SocketChannel_Initialize, connect_return_error__throw_kCantConnectClientToServer_and_close_socket) {
    // Given:
    EXPECT_CALL(mock.startup(), Call(_, _)).Times(AnyNumber()).WillOnce(Return(NO_ERROR));
    EXPECT_CALL(mock.socket(), Call(_, _, _)).Times(AnyNumber());
    ON_CALL(mock.socket(), Call(_, _, _)).WillByDefault(Return(VALID_SOCKET));

    // Given:
    InSequence s;

    // Given:
    EXPECT_CALL(mock.connect(), Call(_, _, _))
        .Times(1)
        .WillOnce(Return(SOCKET_ERROR));

    // Given:
    EXPECT_CALL(mock.close(), Call(Eq(VALID_SOCKET))).Times(1);
    EXPECT_CALL(mock.cleanup(), Call()).Times(1);

    // When:
    ASSERT_THAT(gradle_channel_initialize(config_ref), Eq(kCantConnectClientToServer));

    // Then:
    ASSERT_THAT(socketfd, Eq(INVALID_SOCKET));
}

TEST_F(SocketChannel_Initialize, success__valid_socket) {
    // Given:
    EXPECT_CALL(mock.startup(), Call(_, _)).Times(AnyNumber()).WillOnce(Return(NO_ERROR));
    EXPECT_CALL(mock.close(), Call(_)).Times(0);
    EXPECT_CALL(mock.cleanup(), Call()).Times(0);

    // Given:
    InSequence s;

    // Given:
    EXPECT_CALL(mock.socket(), Call(Eq(AF_INET), Eq(SOCK_STREAM), Eq(IPPROTO_TCP)))
        .Times(1)
        .WillOnce(Return(VALID_SOCKET));

    // Given:
    uint16_t port = 0;
    EXPECT_CALL(mock.connect(), Call(Eq(VALID_SOCKET), NotNull(), Eq(static_cast<socklen_t>(sizeof(struct sockaddr_in)))))
        .Times(1)
        .WillOnce(
            WithArg<1>(
                Invoke(
                    [&] (const struct sockaddr * addr) {
                        port = ntohs(((const struct sockaddr_in *)addr)->sin_port);
                        return 0;
                    }
                )
            )
        );

    // When:
    ASSERT_THAT(gradle_channel_initialize(config_ref), Eq(kSuccess));

    // Then:
    ASSERT_THAT(port, Eq(config.port));
    ASSERT_THAT(socketfd, Eq(VALID_SOCKET));
}




class SocketChannel_Read : public SocketChannel {
  public:
    void SetUp() {
        __super::SetUp();

        socketfd = VALID_SOCKET;
    }
    static const uint16_t length = 1024;
    uint8_t dsts[length];
    int read = 0;
};

TEST_F(SocketChannel_Read, not_initialize__throw_kNotInitialized) {
    // Given:
    socketfd = INVALID_SOCKET;

    // When:
    ASSERT_THAT(gradle_channel_read(dsts, length, &read), Eq(kNotInitialized));
}

TEST_F(SocketChannel_Read, recv_return_error__throw_kReceiveFailed) {
    // Given:
    EXPECT_CALL(mock.recv(), Call(_, _, _, _)).Times(1).WillOnce(Return(SOCKET_ERROR));

    // When:
    ASSERT_THAT(gradle_channel_read(dsts, length, &read), Eq(kReceiveFailed));
}

TEST_F(SocketChannel_Read, null_read_count__throw_kInvalidReadCount) {
    // When:
    ASSERT_THAT(gradle_channel_read(dsts, length, nullptr), Eq(kInvalidReadCount));
}

TEST_F(SocketChannel_Read, recv_succeed__dsts_contains_the_amount_of_data_read) {
    // Given:
    EXPECT_CALL(mock.recv(), Call(Eq(socketfd), Eq(reinterpret_cast<char *>(&dsts[0])), Eq(static_cast<int>(length)), Eq(0))).Times(1).WillOnce(Return(42));

    // When:
    ASSERT_THAT(gradle_channel_read(dsts, length, &read), Eq(kSuccess));

    // Then:
    ASSERT_THAT(read, Eq(42));
}



class SocketChannel_Write : public SocketChannel {
  public:
    void SetUp() {
        __super::SetUp();

        socketfd = VALID_SOCKET;
        (void)memset(srcs, 42, length);
    }
    static const uint16_t length = 1024;
    uint8_t srcs[length];
};

TEST_F(SocketChannel_Write, not_initialize__throw_kNotInitialized) {
    // Given:
    socketfd = INVALID_SOCKET;

    // When:
    ASSERT_THAT(gradle_channel_write(srcs, length), Eq(kNotInitialized));
}

TEST_F(SocketChannel_Write, send_return_error__throw_kSendFailed) {
    // Given:
    EXPECT_CALL(mock.send(), Call(_, _, _, _)).Times(1).WillOnce(Return(SOCKET_ERROR));

    // When:
    ASSERT_THAT(gradle_channel_write(srcs, length), Eq(kSendFailed));
}

TEST_F(SocketChannel_Write, partial_send__throw_kSendFailed) {
    // Given:
    EXPECT_CALL(mock.send(), Call(_, _, _, _)).Times(1).WillOnce(Return(42));

    // When:
    ASSERT_THAT(gradle_channel_write(srcs, length), Eq(kSendFailed));
}

TEST_F(SocketChannel_Write, send_succeed__return_success) {
    // Given:
    EXPECT_CALL(mock.send(), Call(Eq(VALID_SOCKET), Eq(reinterpret_cast<char *>(srcs)), Eq(static_cast<int>(length)), Eq(0))).Times(1).WillOnce(Return(length));

    // When:
    ASSERT_THAT(gradle_channel_write(srcs, length), Eq(kSuccess));
}





class SocketChannel_Shutdown : public SocketChannel {
  public:
    void SetUp() {
        __super::SetUp();

        socketfd = VALID_SOCKET;
    }
};

TEST_F(SocketChannel_Shutdown, not_initialize__throw_kNotInitialized) {
    // Given:
    socketfd = INVALID_SOCKET;

    // When:
    ASSERT_THAT(gradle_channel_shutdown(), Eq(kNotInitialized));
}

TEST_F(SocketChannel_Shutdown, initialized__close_socket_and_cleanup) {
    // Given:
    InSequence s;
    EXPECT_CALL(mock.close(), Call(Eq(socketfd))).Times(1);
    EXPECT_CALL(mock.cleanup(), Call()).Times(1);

    // When:
    ASSERT_THAT(gradle_channel_shutdown(), Eq(kSuccess));
}
