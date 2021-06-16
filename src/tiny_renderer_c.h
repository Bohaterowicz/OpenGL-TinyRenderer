#pragma once
#include <stdint.h>

#define Pi32 3.14159265359f

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#define Kilobytes(Size) ((Size)*1024LL)
#define Megabytes(Size) (Kilobytes(Size) * 1024LL)
#define Gigabytes(Size) (Megabytes(Size) * 1024LL)
#define Terabytes(Size) (Gigabytes(Size) * 1024LL)

#define Minimum(A, B) (((A) < (B)) ? (A) : (B))
#define Maximum(A, B) (((A) > (B)) ? (A) : (B))

#define TRUE 1
#define FALSE 0

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef float real32;
typedef double real64;

typedef int32 bool32;
