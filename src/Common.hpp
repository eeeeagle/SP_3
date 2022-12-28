#ifndef COMMON
#define COMMON

#include "Check.hpp"

#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <vector>
#include <unistd.h>

static double get_time()
{
    struct timeval tv{};
    struct timezone tz{};
    gettimeofday(&tv, &tz);
    return ((double)tv.tv_sec + (double)tv.tv_usec*1e-6);
}

#endif