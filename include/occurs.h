#ifndef OCCURS_H
#define OCCURS_H

#include "stree.h"
#include "types.h"
#include "basedef.h"
#include "bitvector.h"
#include "sort.h"
#include "eval.h"
#include "array.h"

extern Uchar *text, **suffixbase, *sentinel;

extern Uint *stree, rootchildtab[UCHAR_MAX + 1];

extern Bool rootevaluated;


///////////////////////////////////////////////////////////////////////////////
// Macros


// Checks if there is an a-edge out from the root
#define CHECK_A_EDGE\
    {\
        if(IS_LEAF(&rootchild)) {\
            text_probe = text + (rootchild & ~LEAFBIT);\
            Uint prefix = lcp(patt_probe + 1, patt_end, text_probe + 1, sentinel - 1);\
            if ((Uint) (patt_end - patt_probe) == prefix)\
            {\
              return True;\
            }\
            return False;\
        }\
        vertex = stree + rootchild;\
     }


// Tries to match the remainder of the pattern with the current leaf edge
#define MATCH_LEAF_EDGE\
        if(text_probe == sentinel)\
        {\
          return False;\
        }\
        edgechar = *text_probe;\
        if(edgechar == patt_head)\
        {\
          if((Uint) (patt_end - patt_probe) ==\
                     lcp(patt_probe+1,patt_end,text_probe+1,sentinel-1))\
          {\
            return True;\
          }\
          return False;\
        }




///////////////////////////////////////////////////////////////////////////////
// Functions


Bool search(Uchar *left, Uchar *patt_end);

#endif
