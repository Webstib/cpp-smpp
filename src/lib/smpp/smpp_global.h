#ifndef SMPP_GLOBAL_H
#define SMPP_GLOBAL_H

#include <stddef.h>
#include <stdio.h>
#include <string>
#include <list>
#include <boost/asio.hpp>

#if defined(SMPP_EXPORTS)
#  define SMPP_EXPORT __declspec(dllexport)
#elif defined(SMPP_IMPORTS)
#  define SMPP_EXPORT __declspec(dllimport)
#else
#  define SMPP_EXPORT
#endif

typedef unsigned char uint8;
typedef char int8;
typedef short int16;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef int int32;

#endif // SMPP_GLOBAL_H
