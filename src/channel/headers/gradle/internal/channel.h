#ifndef _INTERNAL_CHANNEL_H_
#define _INTERNAL_CHANNEL_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "gradle/channel.h"


#if defined(WIN32)
#  include "WS2tcpip.h"
#elif defined(__linux__)
#  include <unistd.h>
#else
#  error "Invalid operating system, please choose one"
#endif


#if defined(__linux__)
typedef unsigned int SOCKET;
#endif  // defined(__linux__)

#if defined(__linux__)
#  undef INVALID_SOCKET
#  define INVALID_SOCKET (SOCKET)(~0)
#endif // defined(__linux__)

#if defined(__linux__)
typedef void WSADATA;
#  undef NO_ERROR
#  define NO_ERROR (0L)

//int WSAAPI WSACleanup(void) {
//    return NO_ERROR;
//}
//
//int
//WSAAPI
//WSAStartup(
//     uint16_t wVersionRequested,
//     WSADATA * lpWSAData
//    ) {
//    return NO_ERROR;
//}
#endif // defined(__linux__)

#if defined(__linux__)
#  undef SOCKET_ERROR
#  define SOCKET_ERROR (-1)
#endif  // defined(__linux__)

#if defined(__linux__)
#  define closesocket(s) close(s)
#endif  // defined(__linux__)

//#if defined(WIN32)
//typedef int socklen_t;
//#endif  // defined(WIN32)

#define FACILITY_DEFAULT_CHANNEL 1
#define kInvalidReadCount           DECLARE_STATUS_FAILURE(FACILITY_DEFAULT_CHANNEL, 1)
#define kReceiveFailed              DECLARE_STATUS_FAILURE(FACILITY_DEFAULT_CHANNEL, 2)
#define kNotInitialized             DECLARE_STATUS_FAILURE(FACILITY_DEFAULT_CHANNEL, 3)
#define kInvalidIpv4Address         DECLARE_STATUS_FAILURE(FACILITY_DEFAULT_CHANNEL, 4)
#define kStartupFail                DECLARE_STATUS_FAILURE(FACILITY_DEFAULT_CHANNEL, 5)
#define kInvalidChannelType         DECLARE_STATUS_FAILURE(FACILITY_DEFAULT_CHANNEL, 6)
#define kCantCreateSocket           DECLARE_STATUS_FAILURE(FACILITY_DEFAULT_CHANNEL, 7)
#define kCantConnectClientToServer  DECLARE_STATUS_FAILURE(FACILITY_DEFAULT_CHANNEL, 8)
#define kSendFailed                 DECLARE_STATUS_FAILURE(FACILITY_DEFAULT_CHANNEL, 9)

typedef struct {
    gradle_channel_configuration_t header;
    const char * address;
    uint16_t port;
} gradle_socket_channel_configuration_t;

extern SOCKET socketfd;


#ifdef __cplusplus
}
#endif  // __cplusplus

#endif
