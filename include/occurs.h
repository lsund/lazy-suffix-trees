#ifndef OCCURS_H
#define OCCURS_H

#include "stree.h"
#include "types.h"
#include "basedef.h"
#include "bitvector.h"
#include "sort.h"
#include "eval.h"
#include "array.h"
#include "result.h"


typedef struct pattern {
    wchar_t head;
    wchar_t *probe;
    wchar_t *end;
} Pattern;


///////////////////////////////////////////////////////////////////////////////
// Macros


// Checks if there is an a-edge out from the root
#define CHECK_A_EDGE\
    {\
        if(IS_LEAF(&rootchild)) {\
            text_probe = wtext + (rootchild & ~LEAFBIT);\
            Uint prefix = lcp(patt_probe + 1, patt_end, text_probe + 1, sentinel - 1);\
            if ((Uint) (patt_end - patt_probe) == prefix)\
            {\
              return true;\
            }\
            return false;\
        }\
        vertex = stree + rootchild;\
     }


// Tries to match the remainder of the pattern with the current leaf edge
#define MATCH_LEAF_EDGE\
        if(text_probe == sentinel)\
        {\
          return false;\
        }\
        edgechar = *text_probe;\
        if(edgechar == patt_head)\
        {\
          if((Uint) (patt_end - patt_probe) ==\
                     lcp(patt_probe+1,patt_end,text_probe+1,sentinel-1))\
          {\
            return true;\
          }\
          return false;\
        }




///////////////////////////////////////////////////////////////////////////////
// Functions


bool search(wchar_t *left, wchar_t *patt_end);

#endif
