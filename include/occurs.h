#ifndef OCCURS_H
#define OCCURS_H

#include "types.h"
#include "basedef.h"
#include "intbits.h"
#include "sort.h"
#include "eval.h"
#include "arraydef.h"

extern Uchar *text, **suffixbase, *sentinel;

extern Uint *stree, rootchildtab[UCHAR_MAX + 1];

extern BOOL rootevaluated;


///////////////////////////////////////////////////////////////////////////////
// Macros


// Checks if there is an a-edge out from the root
#define CHECK_A_EDGE\
    {\
        Uint rootchild;\
        if((rootchild = rootchildtab[firstchar]) == UNDEFINEDSUCC) {\
            return False;\
        }\
        if(rootchild & LEAFBIT) {\
            lefttext = text + (rootchild & ~LEAFBIT);\
            Uint lcp_res = lcp(probe + 1, rightpattern, lefttext + 1, sentinel - 1);\
            if ((Uint) (rightpattern-probe) == lcp_res)\
            {\
              return True;\
            }\
            return False;\
        }\
        nodeptr = stree + rootchild;\
     }


#define CHECKROOTCHILDWITHPOS\
        {\
          Uint rootchild;\
          if((rootchild = rootchildtab[firstchar]) == UNDEFINEDSUCC)\
          {\
            return False;\
          }\
          if(rootchild & LEAFBIT)\
          {\
            lefttext = text + (rootchild & ~LEAFBIT);\
            if((Uint) (rightpattern-probe) ==\
                    lcp(probe+1,rightpattern,lefttext+1,sentinel-1))\
            {\
              STOREINARRAY(resultpos, Uint,256, rootchild & ~LEAFBIT);\
              return True;\
            }\
            return False;\
          }\
          nodeptr = stree + rootchild;\
        }


// Tries to match the remainder of the pattern with the current leaf edge
#define CHECKLEAFEDGE\
        if(lefttext == sentinel)\
        {\
          return False;\
        }\
        edgechar = *lefttext;\
        if(edgechar == firstchar)\
        {\
          if((Uint) (rightpattern - probe) ==\
                     lcp(probe+1,rightpattern,lefttext+1,sentinel-1))\
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
          if((Uint) (rightpattern - probe) ==\
                  lcp(probe+1,rightpattern,lefttext+1,sentinel-1))\
          {\
            STOREINARRAY(resultpos,Uint,256,(Uint) (lefttext - text));\
            return True;\
          }\
          return False;\
        }

// Tries to match the remainder of the pattern with the current branch edge
#define CHECKBRANCHEDGE\
        prefixlen = UintConst(1)+\
                    lcp(probe+1,rightpattern,lefttext+1,lefttext+edgelen-1);\
        if(prefixlen == edgelen)\
        {\
          probe += edgelen;\
        } else\
        {\
          if(prefixlen == (Uint) (rightpattern - probe + 1))\
          {\
            return True;\
          }\
          return False;\
        }


#define CHECKBRANCHEDGEWITHPOS\
        prefixlen =\
            UintConst(1)+\
            lcp(probe+1,rightpattern,lefttext+1,lefttext+edgelen-1);\
        if(prefixlen == edgelen)\
        {\
          probe += edgelen;\
        } else\
        {\
          if(prefixlen == (Uint) (rightpattern - probe + 1))\
          {\
            collectpositions(resultpos,nodeptr);\
            return True;\
          }\
          return False;\
        }


///////////////////////////////////////////////////////////////////////////////
// Functions

BOOL occurslazy(
        Uchar *text,
        Uint textlen,
        Uchar *leftpattern,
        Uchar *rightpattern
    );


BOOL occurseager(Uchar *text,
                        /*@unused@*/ Uint textlen,
                        Uchar *leftpattern,Uchar *rightpattern);
#endif
