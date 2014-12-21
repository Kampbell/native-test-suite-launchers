#ifndef _GRADLE_THREAD_H_
#define _GRADLE_THREAD_H_

#include "gradle/internal/status.h"

typedef void(*gradle_communication_thread_t)();

status_t gradle_thread_create(gradle_communication_thread_t thread_function);
status_t gradle_thread_stop();
status_t gradle_thread_join();

#endif  // _GRADLE_THREAD_H_
