#ifndef COMMON_H
#define COMMON_H

// set all use boost dynamic library
#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

// set shared buffer default length
#define DEFAULT_SHAREDBUFFER_SIZE       1024*4

// Room server default work thread count
#define DEFAULT_THREAD_COUNT            8

#include "platform.h"
#include "nmdefs.h"

#endif // COMMON_H
