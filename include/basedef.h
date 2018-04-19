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


///////////////////////////////////////////////////////////////////////////////
// Fields

#define ITER                    1

// Each branching vertex requires two integers
#define BRANCHWIDTH             UintConst(2)

#define MAXTEXTLEN              UintConst(715827882)

#define MAXPATTERNLEN           1024

#define MAX_PATTERNS            15000

// undefined reference
#define UNDEFREFERENCE          UINT_MAX

// undefined successor
#define UNDEFINEDSUCC           UINT_MAX

// Transform bytes into megabytes
#define MEGABYTES(V)            ((double) (V)/((UintConst(1) << 20) - 1))

#define MAX_CHARS               10000

// Is a character larger than 255
#define ISSPECIAL(C)    ((C) >= (Uchar) UCHAR_MAX)

#endif
