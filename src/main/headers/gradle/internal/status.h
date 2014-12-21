#ifndef _GRADLE_INTERNAL_STATUS_H_
#define _GRADLE_INTERNAL_STATUS_H_

#include <stdint.h>

typedef int32_t status_t;

#define DECLARE_STATUS_SUCCESS(facility, code) ((status_t)((facility<<16)|code))
#define DECLARE_STATUS_FAILURE(facility, code) ((status_t)((1<<31)|(facility<<16)|code))
#define FACILITY_GENERIC 0

#define kSuccess                DECLARE_STATUS_SUCCESS(FACILITY_GENERIC, 0)
#define kUnsuccessful           DECLARE_STATUS_FAILURE(FACILITY_GENERIC, 0)

#endif  // _GRADLE_INTERNAL_STATUS_H_
