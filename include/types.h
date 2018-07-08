#ifndef TYPES_H
#define TYPES_H
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Primitive type definitions
typedef unsigned char  Uchar;
typedef unsigned short Ushort;
typedef unsigned long  Uint;
typedef signed   long  Sint;
typedef wchar_t        Wchar;
typedef Uint           Vertex;

// Reference definitions
typedef Uint *VertexP;
typedef Wchar *Suffix;

// Unsigned integer constructor
#define UINT(N)   (N##UL)

#endif
