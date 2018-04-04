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

#ifndef ARRAY_H
#define ARRAY_H
#include "types.h"
#include "spaceman.h"
#include "error.h"

// This file declare and manipulate dynamic arrays whose size grow on demand.
// The arrays are defined over some type T and implemented using three
// components:
//
// 1. space - is a pointer to the space block of type T
//
// 2. allocated - is a integer storing the number in the array currently
//                allocated
// 3. nextfree - holds the smallest index of the array where no value is stored

#define CELL_SIZE 256

// Declares a array struct of type T
#define DECLAREARRAYSTRUCT(TYPE)\
        typedef struct\
        {\
          TYPE *space##TYPE;\
          Uint allocated##TYPE, nextfree##TYPE;\
        } Array##TYPE


// Initialize an empty array
#define INITARRAY(A, TYPE)\
        (A)->space##TYPE = NULL;\
        (A)->allocated##TYPE = (A)->nextfree##TYPE = 0


// Copy an array
#define COPYARRAY(A,B)\
        *(A) = *(B)


// Checks if `nextfree` points to an index which is not allocated yet. If this
// is the case, the number of cells allocated is incremented by L.
#define CHECKARRAYSPACE(A,TYPE,L)\
        if((A)->nextfree##TYPE >= (A)->allocated##TYPE)\
        {\
          (A)->allocated##TYPE += L;\
          (A)->space##TYPE\
             = (TYPE *) realloc((A)->space##TYPE,\
                                (size_t) (sizeof(TYPE)*(A)->allocated##TYPE));\
        }\
        NOTSUPPOSEDTOBENULL((A)->space##TYPE)


// Checks the space and delivers a pointer to the next free element in the
// array.
#define GETNEXTFREEINARRAY(P,A,TYPE,L)\
        CHECKARRAYSPACE(A,TYPE,L);\
        P = (A)->space##TYPE + (A)->nextfree##TYPE++;


// Checks the space and stores `V` in the `nextfree` component of the array.
// `nextfree` is incremented.
#define ARRAY_STORE(A, TYPE, L, VAL)\
        CHECKARRAYSPACE(A, TYPE, L);\
        (A)->space##TYPE[(A)->nextfree##TYPE++] = VAL


// Frees the arry if it is not NULL
#define FREEARRAY(A,TYPE)\
        if((A)->space##TYPE != NULL)\
        {\
          FREE((A)->space##TYPE);\
        }


// Declare some arrys for common types.
DECLAREARRAYSTRUCT(Uchar);
DECLAREARRAYSTRUCT(Ushort);
DECLAREARRAYSTRUCT(char);
DECLAREARRAYSTRUCT(Uint);
DECLAREARRAYSTRUCT(Sint);
DECLAREARRAYSTRUCT(PairUint);
DECLAREARRAYSTRUCT(ThreeUint);
DECLAREARRAYSTRUCT(Fileinfo);

#endif
