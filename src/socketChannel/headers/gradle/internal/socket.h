#ifndef _GRADLE_INTERNAL_SOCKET_H_
#define _GRADLE_INTERNAL_SOCKET_H_

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

#endif  // _GRADLE_INTERNAL_SOCKET_H_
