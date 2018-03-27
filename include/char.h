/*
 * Copyright by Stefan Kurtz (C) 2000-2003
 * =====================================
 * You may use, copy and distribute this file freely as long as you
 * - do not change the file,
 * - leave this copyright notice in the file,
 * - do not make any profit with the distribution of this file
 * - give credit where credit is due
 * You are not allowed to copy or distribute this file otherwise
 * The commercial usage and distribution of this file is prohibited
 * Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
 */
/*
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */


#ifndef CHAR_H
#define CHAR_H
#include <limits.h>


// separator symbol in multiple seq
#define SEPARATOR       UCHAR_MAX


// wildcard symbol in multiple seq
#define WILDCARD        (SEPARATOR - 1)


// undefined character in multiple seq
#define UNDEFCHAR       (SEPARATOR - 2)


// either WILDCARD or SEPARATOR
#define ISSPECIAL(C)    ((C) >= (Uchar) WILDCARD)


// neither WILDCARD nor SEPARATOR
#define ISNOTSPECIAL(C) ((C) < (Uchar) WILDCARD)

#endif