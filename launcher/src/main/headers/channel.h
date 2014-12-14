#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <stdint.h>

typedef uint8_t ECHANNEL_t;

typedef struct {
    uint8_t type;
} gradle_channel_configuration_t;

ECHANNEL_t gradle_channel_initialize(gradle_channel_configuration_t * const configuration);
ECHANNEL_t gradle_channel_peek(uint32_t * const size);
ECHANNEL_t gradle_channel_read(uint8_t * const dsts, uint32_t offset, uint16_t length);
ECHANNEL_t gradle_channel_write(uint8_t * const srcs, uint32_t offset, uint16_t length);
ECHANNEL_t gradle_channel_shutdown();

#endif  // _CHANNEL_H_

