#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stdint.h>

#include "WS2tcpip.h"

typedef uint32_t ECHANNEL_t;

extern const uint8_t kSocketChannel;

#define DECLARE_STATUS_SUCCESS(facility, code) ((ECHANNEL_t)((facility<<16)|code))
#define DECLARE_STATUS_FAILURE(facility, code) ((ECHANNEL_t)((1<<31)|(facility<<16)|code))
#define FACILITY_GENERIC 0
#define FACILITY_CHANNEL 1

#define kSuccess                DECLARE_STATUS_SUCCESS(FACILITY_GENERIC, 0)
#define kUnsuccessful           DECLARE_STATUS_FAILURE(FACILITY_GENERIC, 0)

#define kInvalidSocketFunction  DECLARE_STATUS_FAILURE(FACILITY_CHANNEL, 1)
#define kInvalidConnectFunction DECLARE_STATUS_FAILURE(FACILITY_CHANNEL, 2)
#define kInvalidCloseFunction   DECLARE_STATUS_FAILURE(FACILITY_CHANNEL, 3)
#define kInvalidReceiveFunction DECLARE_STATUS_FAILURE(FACILITY_CHANNEL, 4)
#define kInvalidSendFunction    DECLARE_STATUS_FAILURE(FACILITY_CHANNEL, 5)
#define kInvalidChannelType     DECLARE_STATUS_FAILURE(FACILITY_CHANNEL, 6)
#define kCantCreateSocket       DECLARE_STATUS_FAILURE(FACILITY_CHANNEL, 7)
#define kCantConnectClientToServer DECLARE_STATUS_FAILURE(FACILITY_CHANNEL, 8)

typedef int(socket_t)(int socket_family, int socket_type, int protocol);
typedef int(connect_t)(int socket, const struct sockaddr * addr, socklen_t addrlen);
typedef int(close_t)(int socket);
typedef size_t(recv_t)(int socket, void * buffer, size_t length, int flags);
typedef size_t(send_t)(int socket, void * buffer, size_t length, int flags);

typedef struct {
    uint8_t type;
	socket_t * socket;
	connect_t * connect;
	close_t * close;
	recv_t * recv;
	send_t * send;
} gradle_channel_configuration_t;

ECHANNEL_t gradle_channel_initialize(gradle_channel_configuration_t * const configuration);
ECHANNEL_t gradle_channel_peek(uint32_t * const size);
ECHANNEL_t gradle_channel_read(uint8_t * const dsts, uint16_t length, uint16_t * const read_length);
ECHANNEL_t gradle_channel_write(uint8_t * const srcs, uint16_t length);
ECHANNEL_t gradle_channel_shutdown();

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // _CHANNEL_H_

