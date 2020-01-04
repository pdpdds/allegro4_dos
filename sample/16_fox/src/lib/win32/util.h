#ifndef UTIL_H
#define UTIL_H

#ifdef _WIN32
#include <stdint.h> // portable: uint64_t   MSVC: __int64 

int gettimeofday(struct timeval * tp, struct timezone * tzp);
#endif
#endif