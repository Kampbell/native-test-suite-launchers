#ifndef _INTERNAL_CHANNEL_H_
#define _INTERNAL_CHANNEL_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "gradle/channel.h"
#include "gradle/internal/socket.h"


#define FACILITY_SOCKET_CHANNEL 1
#define kInvalidReadCount           DECLARE_STATUS_FAILURE(FACILITY_SOCKET_CHANNEL, 1)
#define kReceiveFailed              DECLARE_STATUS_FAILURE(FACILITY_SOCKET_CHANNEL, 2)
#define kNotInitialized             DECLARE_STATUS_FAILURE(FACILITY_SOCKET_CHANNEL, 3)
#define kInvalidIpv4Address         DECLARE_STATUS_FAILURE(FACILITY_SOCKET_CHANNEL, 4)
#define kStartupFail                DECLARE_STATUS_FAILURE(FACILITY_SOCKET_CHANNEL, 5)
#define kInvalidChannelType         DECLARE_STATUS_FAILURE(FACILITY_SOCKET_CHANNEL, 6)
#define kCantCreateSocket           DECLARE_STATUS_FAILURE(FACILITY_SOCKET_CHANNEL, 7)
#define kCantConnectClientToServer  DECLARE_STATUS_FAILURE(FACILITY_SOCKET_CHANNEL, 8)
#define kSendFailed                 DECLARE_STATUS_FAILURE(FACILITY_SOCKET_CHANNEL, 9)

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
