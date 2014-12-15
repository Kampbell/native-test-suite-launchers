#ifndef _INTERNAL_CHANNEL_H_
#define _INTERNAL_CHANNEL_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "channel.h"

#include "WS2tcpip.h"

extern SOCKET socketfd;
extern gradle_channel_configuration_t * g_context;

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif
