#include "channel.h"

//#include <sys/socket.h>
#define WIN32_LEAN_AND_MEAN
#include "winsock2.h"
#include "Ws2tcpip.h"

SOCKET socketfd = INVALID_SOCKET;

ECHANNEL_t gradle_channel_initialize(gradle_channel_configuration_t * const configuration)
{
    WSADATA winsock_detail;
    stuct sockaddr_in client_service;
    int result = -1;

    memset(&client_service, 0, sizeof(client_service));
    client_service.sin_family = AF_INET;
    client_service.sin_addr.s_addr = inet_addr("127.0.0.1");  // TODO(daniel): Take the one from configuration
    client_service.sin_port = htons(5000);  // TODO(daniel): Take the one from configuration

    if (kWin32SocketType != configuration->type)
    {
        // Invalid parameter
    }
    else if (NO_ERROR != WSAStartup(MAKEWORD(2,2), &winsock_detail))
    {

    }
    else if (INVALID_SOCKET == (socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
    {

    }
    else if (SOCKET_ERROR == connect(socketfd, (SOCKADDR*)&client_service, sizeof(client_service)))
    {

    }
    else
    {
        result = 0;
    }


    // Cleanup
    if (0 > result && INVALID_SOCKET != socket)
    {
        closesocket(socketfd);
    }
    if (0 > result)
    {
        WSACleanup();
    }

    return result;
}

ECHANNEL_t gradle_channel_peek(uint32_t * const size)
{
    int result = -1;

    if (NULL == size)
    {
        // Invalide parameters
    }
    else if (SOCKET_ERROR == (*size = recv(sockfd, NULL, 0, MSG_PEEK)))
    {
        // Problem
    }
    else
    {
        result = 0;
    }

    return result;
}

ECHANNEL_t gradle_channel_read(uint8_t * const dsts, uint16_t length, uint16_t* read_length)
{
    int result = -1;
    int read_len = 0;

    if (SOCKET_ERROR == (read_len = recv(socketfd, dsts, length, 0)))
    {
    }
    else
    {
        result = 0;
    }

    if (NULL != read_length)
    {
        *read_length = read_len;
    }

    return result;
}

ECHANNEL_t gradle_channel_write(uint8_t * const srcs, uint16_t size)
{
    int result = -1;
    int write_length = 0;

    if (SOCKET_ERROR == (write_length = send(sockfd, srcs, size, 0)))
    {
        // Problem
    }
    else if (size != write_length)
    {
        // Didn't write all the information
    }
    else
    {
        result = 0;
    }

    return result;
}

ECHANNEL_t gradle_channel_shutdown()
{
    int result = -1;

    if (INVALID_SOCKET == socketfd)
    {

    }
    else
    {
        closesocket(sockfd);
        WSACleanup();
        result = 0;
    }

    return result;
}

