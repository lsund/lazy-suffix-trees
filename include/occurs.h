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


#define CHECK_EMPTY\
    if(left > right) {\
        return True;\
    } else {\
        probe = left;\
        firstchar = *left;\
    }\


// Checks if there is an a-edge out from the root
#define CHECK_A_EDGE\
    {\
        Uint rootchild;\
        if((rootchild = rootchildtab[firstchar]) == UNDEFINEDSUCC) {\
            return False;\
        }\
        if(rootchild & LEAFBIT) {\
            lefttext = text + (rootchild & ~LEAFBIT);\
            Uint lcp_res = lcp(probe + 1, right, lefttext + 1, sentinel - 1);\
            if ((Uint) (right - probe) == lcp_res)\
            {\
              return True;\
            }\
            return False;\
        }\
        vertex = stree + rootchild;\
     }


#define CHECK_A_EDGE_POS\
        {\
          Uint rootchild;\
          if((rootchild = rootchildtab[firstchar]) == UNDEFINEDSUCC)\
          {\
            return False;\
          }\
          if(rootchild & LEAFBIT)\
          {\
            lefttext = text + (rootchild & ~LEAFBIT);\
            if((Uint) (right-probe) ==\
                    lcp(probe+1,right,lefttext+1,sentinel-1))\
            {\
              ARRAY_STORE(resultpos, Uint,256, rootchild & ~LEAFBIT);\
              return True;\
            }\
            return False;\
          }\
          vertex = stree + rootchild;\
        }


// Tries to match the remainder of the pattern with the current leaf edge
#define MATCH_LEAF_EDGE\
        if(lefttext == sentinel)\
        {\
          return False;\
        }\
        edgechar = *lefttext;\
        if(edgechar == firstchar)\
        {\
          if((Uint) (right - probe) ==\
                     lcp(probe+1,right,lefttext+1,sentinel-1))\
          {\
            return True;\
          }\
          return False;\
        }


#define CHECKLEAFEDGEWITHPOS\
        if(lefttext == sentinel)\
        {\
          return False;\
        }\
        edgechar = *lefttext;\
        if(edgechar == firstchar)\
        {\
          if((Uint) (right - probe) ==\
                  lcp(probe+1,right,lefttext+1,sentinel-1))\
          {\
            ARRAY_STORE(resultpos, Uint, 256, (Uint) (lefttext - text));\
            return True;\
          }\
          return False;\
        }

// Tries to match the remainder of the pattern with the current branch edge
#define CHECKBRANCHEDGE\
        prefixlen = UintConst(1)+\
                    lcp(probe+1,right,lefttext+1,lefttext+edgelen-1);\
        if(prefixlen == edgelen)\
        {\
          probe += edgelen;\
        } else\
        {\
          if(prefixlen == (Uint) (right - probe + 1))\
          {\
            return True;\
          }\
          return False;\
        }


#define CHECKBRANCHEDGEWITHPOS\
        prefixlen =\
            UintConst(1)+\
            lcp(probe+1,right,lefttext+1,lefttext+edgelen-1);\
        if(prefixlen == edgelen)\
        {\
          probe += edgelen;\
        } else\
        {\
          if(prefixlen == (Uint) (right - probe + 1))\
          {\
            generate_lps(resultpos,vertex);\
            return True;\
          }\
          return False;\
        }


///////////////////////////////////////////////////////////////////////////////
// Functions


Bool search_lazy(Uchar *left, Uchar *right);

Bool search_eager(Uchar *left, Uchar *right);

#endif
