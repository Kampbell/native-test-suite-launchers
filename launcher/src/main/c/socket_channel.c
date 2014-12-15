#include "channel.h"
#include "internal/channel-impl.h"

//#include <sys/socket.h>
#define WIN32_LEAN_AND_MEAN
#include "Winsock2.h"

const uint8_t kSocketChannel = 1;

SOCKET socketfd = INVALID_SOCKET;
gradle_channel_configuration_t * g_context;

ECHANNEL_t gradle_channel_initialize(gradle_channel_configuration_t * const context) {
    WSADATA winsock_detail;
    struct sockaddr_in client_service;
    int result = -1;

    memset(&client_service, 0, sizeof(client_service));
    client_service.sin_family = AF_INET;
    client_service.sin_addr.s_addr = inet_addr("127.0.0.1");  // TODO(daniel): Take the one from configuration
    client_service.sin_port = htons(5000);  // TODO(daniel): Take the one from configuration

    if (NULL == context->socket) {
        result = kInvalidSocketFunction;
    } else if (NULL == context->connect) {
        result = kInvalidConnectFunction;
    } else if (NULL == context->close) {
        result = kInvalidCloseFunction;
    } else if (NULL == context->recv) {
        result = kInvalidReceiveFunction;
    } else if (NULL == context->send) {
        result = kInvalidSendFunction;
    } else if (kSocketChannel != context->type) {
        result = kInvalidChannelType;
    } else if (NO_ERROR != WSAStartup(MAKEWORD(2,2), &winsock_detail)) {
        // TODO(daniel): Test this case
    } else if (INVALID_SOCKET == (socketfd = context->socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))) {
        result = kCantCreateSocket;
    } else if (SOCKET_ERROR == context->connect(socketfd, (SOCKADDR*)&client_service, sizeof(client_service))) {
        result = kCantConnectClientToServer;
    } else {
        g_context = context;
        result = 0;
    }


    // Cleanup
    if (0 > result && INVALID_SOCKET != socketfd) {
        context->close(socketfd);
        socketfd = INVALID_SOCKET;
    }
	if (0 > result) {
        // TODO(daniel): Test this
        WSACleanup();
    }

    return result;
}

ECHANNEL_t gradle_channel_peek(uint32_t * const size)
{
    int result = -1;

    if (NULL == size) {
        // Invalide parameters
    } else if (SOCKET_ERROR == (*size = g_context->recv(socketfd, NULL, 0, MSG_PEEK))) {
        // Problem
    } else {
        result = 0;
    }

    return result;
}

ECHANNEL_t gradle_channel_read(uint8_t * const dsts, uint16_t length, uint16_t * const read_length) {
    int result = -1;
    int read_len = 0;

    if (SOCKET_ERROR == (read_len = g_context->recv(socketfd, dsts, length, 0))) {
    } else {
        result = 0;
    }

    if (NULL != read_length) {
        *read_length = read_len;
    }

    return result;
}

ECHANNEL_t gradle_channel_write(uint8_t * const srcs, uint16_t size) {
    int result = -1;
    int write_length = 0;

    if (SOCKET_ERROR == (write_length = g_context->send(socketfd, srcs, size, 0))) {
        // Problem
    } else if (size != write_length) {
        // Didn't write all the information
    } else {
        result = 0;
    }

    return result;
}

ECHANNEL_t gradle_channel_shutdown() {
    int result = -1;

    if (INVALID_SOCKET == socketfd) {

    } else {
        g_context->close(socketfd);
        WSACleanup();
        result = 0;
    }

    return result;
}

