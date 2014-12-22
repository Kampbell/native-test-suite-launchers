#ifndef _GRADLE_NATIVE_THREAD_H_
#define _GRADLE_NATIVE_THREAD_H_

#include "gradle/thread.h"

#define FACILITY_NATIVE_THREAD 2
#define kInvalidReadCount           DECLARE_STATUS_FAILURE(FACILITY_SOCKET_CHANNEL, 1)
#define kReceiveFailed              DECLARE_STATUS_FAILURE(FACILITY_SOCKET_CHANNEL, 2)

#endif  // _GRADLE_NATIVE_THREAD_H_
