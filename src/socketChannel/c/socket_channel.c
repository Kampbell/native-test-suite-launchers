#include "gradle/channel.h"
#include "gradle/internal/socket_channel.h"

//#include <sys/socket.h>


const uint8_t kSocketChannel = 1;

SOCKET socketfd = INVALID_SOCKET;

status_t gradle_channel_initialize(gradle_channel_configuration_t * const context) {
#if defined(WIN32)
    WSADATA winsock_detail;
    WSADATA * detail = &winsock_detail;
    uint16_t version = MAKEWORD(2, 2);
#else
    WSADATA * detail = NULL;
    uint16_t version = 0;
#endif  // defined(WIN32)

    struct sockaddr_in client_service;
    gradle_socket_channel_configuration_t * socket_context = (gradle_socket_channel_configuration_t *)context;
    status_t result = kUnsuccessful;

    memset(&client_service, 0, sizeof(client_service));
    client_service.sin_family = AF_INET;

    if (kSocketChannel != context->type) {
        result = kInvalidChannelType;
    } else if (1 != inet_pton(AF_INET, socket_context->address, &client_service.sin_addr.s_addr)) {
        result = kInvalidIpv4Address;
    } else if (NO_ERROR != WSAStartup(version, detail)) {
        result = kStartupFail;
    } else if (INVALID_SOCKET == (socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))) {
        result = kCantCreateSocket;
    } else if (0 == (client_service.sin_port = htons(socket_context->port))
            || SOCKET_ERROR == connect(socketfd, (struct sockaddr *)&client_service, sizeof(client_service))) {
        result = kCantConnectClientToServer;
    } else {
        result = 0;
    }


    // Cleanup
    if (0 > result && INVALID_SOCKET != socketfd) {
        closesocket(socketfd);
        socketfd = INVALID_SOCKET;
    }
	if (0 > result) {
        (void)WSACleanup();
    }

    return result;
}

status_t gradle_channel_peek(uint32_t * const size)
{
    int result = -1;

    if (NULL == size) {
        // Invalide parameters
    } else if (SOCKET_ERROR == (*size = recv(socketfd, NULL, 0, MSG_PEEK))) {
        // Problem
    } else {
        result = 0;
    }

    return result;
}

status_t gradle_channel_read(uint8_t * const dsts, uint16_t length, int * const read_length) {
    status_t result = kUnsuccessful;
    int read_len = 0;

    if (INVALID_SOCKET == socketfd) {
        result = kNotInitialized;
    } else if (NULL == read_length) {
        result = kInvalidReadCount;
    } else if (SOCKET_ERROR == (read_len = recv(socketfd, (char *)dsts, (int)length, 0))) {
        result = kReceiveFailed;
    } else {
        *read_length = read_len;
        result = kSuccess;
    }

    return result;
}

status_t gradle_channel_write(const uint8_t * const srcs, uint16_t size) {
    status_t result = kUnsuccessful;
    int write_length = 0;

    if (INVALID_SOCKET == socketfd) {
        result = kNotInitialized;
    } else if (SOCKET_ERROR == (write_length = send(socketfd, (const char *)srcs, size, 0))) {
        result = kSendFailed;
    } else if (size != write_length) {
        result = kSendFailed;
    } else {
        result = kSuccess;
    }

    return result;
}

status_t gradle_channel_shutdown() {
    status_t result = kUnsuccessful;

    if (INVALID_SOCKET == socketfd) {
        result = kNotInitialized;
    } else {
        (void)closesocket(socketfd);
        (void)WSACleanup();
        result = kSuccess;
    }

    return result;
}
