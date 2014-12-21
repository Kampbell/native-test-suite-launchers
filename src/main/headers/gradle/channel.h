#ifndef _GRADLE_CHANNEL_H_
#define _GRADLE_CHANNEL_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stdint.h>

#include "gradle/internal/status.h"

extern const uint8_t kSocketChannel;

typedef struct {
    uint8_t type;
} gradle_channel_configuration_t;

status_t gradle_channel_initialize(gradle_channel_configuration_t * const configuration);
status_t gradle_channel_peek(uint32_t * const size);
status_t gradle_channel_read(uint8_t * const dsts, uint16_t length, int * const read_length);
status_t gradle_channel_write(const uint8_t * const srcs, uint16_t length);
status_t gradle_channel_shutdown();

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // _GRADLE_CHANNEL_H_
