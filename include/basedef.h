/*
 * Copyright by Stefan Kurtz (C) 1999-2003
 * =====================================
 * You may use, copy and distribute this file freely as long as you
 * - do not change the file,
 * - leave this copyright notice in the file,
 * - do not make any profit with the distribution of this file
 * - give credit where credit is due
 * You are not allowed to copy or distribute this file otherwise
 * The commercial usage and distribution of this file is prohibited
 * Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
 *
 * ======================================
 *
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */

#ifndef PREPROCDEF_H
#define PREPROCDEF_H

#define STREEALLOC              100000000

#define MAXTEXTLEN              UINT(715827882)

#define MAXPATTERNLEN           100000

#define MAX_PATTERNS            350000

// undefined reference
#define UNDEFREFERENCE          UINT_MAX

// undefined successor
#define UNDEF           UINT_MAX

// Transform bytes into megabytes
#define MEGABYTES(V)            ((double) (V)/((UINT(1) << 20) - 1))

// The maximum number of distinct characters allowed in the text. Arrays of
// this size will be repeatedly allocated.
#define MAX_CHARS               330
/* #define MAX_CHARS               10000 */

// Is a character larger than 255
#define ISSPECIAL(C)    ((C) >= (Uchar) UCHAR_MAX)

///////////////////////////////////////////////////////////////////////////////
// Bitvector

// Number of bits in in integer
#define INT_WORDSIZE (UINT(1) << LOGWORDSIZE)

// The value represented by the most significant bit in the INT_WORDSIZE
// binary number, for example, INT_WORDSIZE = 64 on a 64 bit architecture.
// Since it is only the first bit, the value is 2 ^ (64 - 1)
#define MSB (UINT(1) << (INT_WORDSIZE - 1))

// The value represented by the second most significant bit.
#define SECOND_MSB (MSB >> 1)

#endif
