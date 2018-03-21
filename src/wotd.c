#include "wotd.h"

Uchar *text,                   // points to input string \(t\) of length \(n\)
      *sentinel,               // points to \(t[n]\) which is undefined
      characters[UCHAR_MAX + 1], // characters in \(t\) in alphabetical order
      **suffixes,              // array of pointers to suffixes of \(t\)
      **suffixbase,            // pointers into suffixes are w.r.t.\ this var
      **sbuffer,               // buffer to sort suffixes in \texttt{sortByChar}
      **sbufferspace = NULL;  // space to be used by \texttt{sbuffer}

Uint lastrootchild, maxwidth, branchcount, leafcount;

Uint  textlen,                 // length of \(t\)
      maxstacksize,
      alphasize,               // size of alphabet \(\Sigma\)
      alphaindex[UCHAR_MAX + 1], // index of characters in \(\Sigma\)
      occurrence[UCHAR_MAX + 1], // number of occurrences of character
      *streetab = NULL,        // table to hold suffix tree representation
      streetabsize,            // number of integers in \texttt{streetab} allocated
      *nextfreeentry,          // pointer to next unused element in \texttt{streetab}
      sbufferwidth,            // number of elements in \texttt{sbufferspace}
      maxsbufferwidth,         // maximal number of elements in \texttt{sbufferspace}
      suffixessize,            // number of unprocessed suffixes (for eager)
      maxunusedsuffixes,       // when reached, then move and halve space for suffixes
      rootchildtab[UCHAR_MAX+1]; // constant time access to successors of \emph{root}


#define UNDEFINEDSUCC  (UINT_MAX)    // undefined successor

BOOL  rootevaluated;   // flag indicating that the root has been evaluated

#define MAXSUCCSPACE            (BRANCHWIDTH * (UCHAR_MAX+1) + 1)

static void allocstreetab(void)
{
  Uint tmpindex = NODEINDEX(nextfreeentry);
  if(tmpindex >= streetabsize)
  {
    streetabsize += (textlen/10);
    ALLOC(streetab, streetab, Uint, streetabsize + MAXSUCCSPACE);
    // update necessary, since streetab may have been moved.
    nextfreeentry = streetab + tmpindex;
  }
}

static Uint grouplcp(Uchar **left,Uchar **right)
{
  Uchar cmpchar, **i;
  Uint j;

  for(j=UintConst(1); /* nothing */; j++)
  {
    if(*right+j == sentinel)
    {
      return j;
    }
    cmpchar = *(*left+j);
    for(i=left+1; i<=right; i++)
    {
      if(*(*i+j) != cmpchar)
      {
        return j;
      }
    }
  }
}

static Uint evalsuccedges(Uchar **left,Uchar **right)
{
  Uchar firstchar, **r, **l;
  Uint leafnum, firstbranch = UNDEFREFERENCE, *previousnode = NULL;
  BOOL sentineledge = False;

  allocstreetab();
  if(*right == sentinel)
  {
    right--;  // skip the smallest suffix
    sentineledge = True;
  }
  for(l=left; l<=right; l=r+1)
  {
    for(firstchar=**l,r=l; r<right && **(r+1)==firstchar; r++)
    {
      /* nothing */ ;
    }
    previousnode = nextfreeentry;
    // create branching node
    if(r > l)
    {
      if(firstbranch == UNDEFREFERENCE)
      {
        firstbranch = NODEINDEX(nextfreeentry);
      }
      STOREBOUNDARIES(nextfreeentry,l,r);
      // store l and r. resume later with this unevaluated node
      nextfreeentry += BRANCHWIDTH;
      DEBUGCODE(1,branchcount++);
    } else // create leaf
    {
      leafnum = SUFFIXNUMBER(l);
      SETLEAF(nextfreeentry,leafnum);
      nextfreeentry++;
      DEBUGCODE(1,leafcount++);
    }
  }
  if(sentineledge)
  {
    leafnum = SUFFIXNUMBER(right+1);
    SETLEAF(nextfreeentry,leafnum);
    previousnode = nextfreeentry++;
    DEBUGCODE(1,leafcount++);
  }
  NOTSUPPOSEDTOBENULL(previousnode);
  *previousnode |= RIGHTMOSTCHILDBIT;
  return firstbranch;
}

static Uint evalrootsuccedges(Uchar **left,Uchar **right)
{
  Uchar firstchar, **r, **l;
  Uint *rptr, leafnum, firstbranch = UNDEFREFERENCE;

  for(rptr = rootchildtab; rptr <= rootchildtab + UCHAR_MAX; rptr++)
  {
    *rptr = UNDEFINEDSUCC;
  }
  for(l=left; l<=right; l=r+1) // first phase
  {
    for(firstchar=**l,r=l; r<right && **(r+1)==firstchar; r++)
    {
      /* nothing */ ;
    }
    if(r > l) // create branching node
    {
      if(firstbranch == UNDEFREFERENCE)
      {
        firstbranch = NODEINDEX(nextfreeentry);
      }
      STOREBOUNDARIES(nextfreeentry,l,r);
      // store l and r. resume later with this unevaluated branch node
      rootchildtab[firstchar] = NODEINDEX(nextfreeentry);
      DEBUGCODE(1,lastrootchild = NODEINDEX(nextfreeentry));
      nextfreeentry += BRANCHWIDTH;
      DEBUGCODE(1,branchcount++);
    } else // create leaf
    {
      leafnum = SUFFIXNUMBER(l);
      SETLEAF(nextfreeentry,leafnum);
      rootchildtab[firstchar] = leafnum | LEAFBIT;
      nextfreeentry++;
      DEBUGCODE(1,leafcount++);
    }
  }
  SETLEAF(nextfreeentry,textlen | RIGHTMOSTCHILDBIT);
  nextfreeentry++;
  DEBUGCODE(1,leafcount++);
  return firstbranch;
}

static Uint evaluatenodeeager(Uint node)
{
  Uint prefixlen, *nodeptr, unusedsuffixes;
  Uchar **left, **right;

  DEBUG1(3,"#evaluatenodeeager(%lu)\n",(Showuint) node);
  nodeptr = streetab + node;
  left = GETLEFTBOUNDARY(nodeptr);
  right = GETRIGHTBOUNDARY(nodeptr);
  SETLP(nodeptr,SUFFIXNUMBER(left));
  SETFIRSTCHILD(nodeptr,NODEINDEX(nextfreeentry));

  unusedsuffixes = (Uint) (left - suffixes);
  if(suffixessize > UintConst(10000) && unusedsuffixes > maxunusedsuffixes)
  {
    Uint tmpdiff, width = (Uint) (right - left + 1);
    Uchar **i, **j;
    for(i=left, j=suffixes; i<suffixes+suffixessize; i++, j++)
    {
      *j = *i;  // move remaining suffixes to the left
    }
    suffixessize -= unusedsuffixes;
    maxunusedsuffixes = suffixessize >> 1;
    tmpdiff = (Uint) (suffixes - suffixbase);
    DEBUG2(2,"#move[%lu,%lu] ",(Showuint) (left-suffixes),
                               (Showuint) (left-suffixes+suffixessize));
    DEBUG1(2,"[0,%lu]\n",(Showuint) suffixessize);
    ALLOC(suffixes,suffixes,Uchar *,suffixessize);
    suffixbase = suffixes - (tmpdiff + unusedsuffixes);
    left = suffixes;
    right = suffixes + width - 1;
  }
  sbuffer = getsbufferspaceeager(left, right);
  prefixlen = grouplcp(left,right);
  sortByChar(left,right,prefixlen);
  return evalsuccedges(left,right);
}

static void evaluatenodelazy(Uint node)
{
  Uint prefixlen, *nodeptr;
  Uchar **left, **right;

  DEBUG1(3,"#evaluatenodelazy(%lu)\n",(Showuint) node);
  nodeptr = streetab + node;
  left = GETLEFTBOUNDARY(nodeptr);
  right = GETRIGHTBOUNDARY(nodeptr);
  SETLP(nodeptr,SUFFIXNUMBER(left));
  SETFIRSTCHILD(nodeptr,NODEINDEX(nextfreeentry));

  sbuffer = getsbufferspacelazy(left,right);
  prefixlen = grouplcp(left,right);
  sortByChar(left,right,prefixlen);
  (void) evalsuccedges(left,right);
}

static Uint getnextbranch(Uint previousbranch)
{
  Uint *nodeptr = streetab + previousbranch;

  if(ISRIGHTMOSTCHILD(nodeptr))
  {
    return UNDEFREFERENCE;
  }
  nodeptr += BRANCHWIDTH;
  while(True)
  {
    if(ISLEAF(nodeptr))
    {
      if(ISRIGHTMOSTCHILD(nodeptr))
      {
        return UNDEFREFERENCE;
      }
      nodeptr++;
    } else
    {
      return NODEINDEX(nodeptr);
    }
  }
}

#define NOTSTACKEMPTY         (stacktop > 0)
#define PUSHNODE(N)\
        if(stacktop >= stackalloc)\
        {\
          stackalloc += 100;\
          ALLOC(stack,stack,Uint,stackalloc);\
        }\
        DEBUGCODE(1,if(stacktop > maxstacksize) { maxstacksize = stacktop;});\
        NOTSUPPOSEDTOBENULL(stack);\
        stack[stacktop++] = N

#define POPNODE(N)\
        N = stack[--stacktop]

void evaluateeager(void)
{
  Uint firstbranch, nextbranch, node, stacktop=0, stackalloc=0, *stack = NULL;

  sortByChar0();
  firstbranch = evalrootsuccedges(suffixes,suffixes+textlen-1);
  if(firstbranch != UNDEFREFERENCE)
  {
    PUSHNODE(firstbranch);
    while(NOTSTACKEMPTY)
    {
      POPNODE(node);
      while(node != UNDEFREFERENCE)
      {
        if((nextbranch = getnextbranch(node)) != UNDEFREFERENCE)
        {
          PUSHNODE(nextbranch);
        }
        node = evaluatenodeeager(node);
      }
    }
    FREESPACE(stack);
  }
}

void inittree(void)
{
  Uint i;

  DEBUGCODE(1,maxstacksize=maxwidth=branchcount=leafcount=0);
  getUchars(text, textlen, characters, &alphasize);
  sentinel = text+textlen;
  streetabsize = BRANCHWIDTH;
  ALLOC(streetab,streetab,Uint,streetabsize + MAXSUCCSPACE);
  nextfreeentry = streetab;
  suffixessize = textlen+1;
  maxunusedsuffixes = suffixessize >> 1;
  ALLOC(suffixes,NULL,Uchar *,suffixessize);
  suffixbase = suffixes;
  sbufferwidth = 0;
  maxsbufferwidth = textlen >> 8;
  rootevaluated = False;
  for(i=0; i<alphasize; i++)
  {
    alphaindex[(Uint) characters[i]] = i;
  }
  for(i=0; i<=UCHAR_MAX; i++)
  {
    occurrence[i] = 0;
  }
}

static Uint lcp(Uchar *start1,Uchar *end1,Uchar *start2,Uchar *end2)
{
  Uchar *ptr1 = start1, *ptr2 = start2;

  while(ptr1 <= end1 && ptr2 <= end2 && *ptr1 == *ptr2)
  {
    ptr1++;
    ptr2++;
  }
  return (Uint) (ptr1-start1);
}

#define CHECKROOTCHILD\
        {\
          Uint rootchild;\
          if((rootchild = rootchildtab[firstchar]) == UNDEFINEDSUCC)\
          {\
            return False;\
          }\
          if(rootchild & LEAFBIT)\
          {\
            lefttext = text + (rootchild & ~LEAFBIT);\
            if((Uint) (rightpattern-lpatt) == lcp(lpatt+1,rightpattern,lefttext+1,sentinel-1))\
            {\
              return True;\
            }\
            return False;\
          }\
          nodeptr = streetab + rootchild;\
        }


#define CHECKLEAFEDGE\
        if(lefttext == sentinel)\
        {\
          return False;\
        }\
        edgechar = *lefttext;\
        if(edgechar == firstchar)\
        {\
          if((Uint) (rightpattern - lpatt) == lcp(lpatt+1,rightpattern,lefttext+1,sentinel-1))\
          {\
            return True;\
          }\
          return False;\
        }

#define CHECKBRANCHEDGE\
        prefixlen = UintConst(1)+lcp(lpatt+1,rightpattern,lefttext+1,lefttext+edgelen-1);\
        if(prefixlen == edgelen)\
        {\
          lpatt += edgelen;\
        } else\
        {\
          if(prefixlen == (Uint) (rightpattern - lpatt + 1))\
          {\
            return True;\
          }\
          return False;\
        }

static Uint firstchildlp(Uint *nodeptr)
{
  Uint *firstchildptr = streetab + GETFIRSTCHILD(nodeptr);

  if(!ISLEAF(firstchildptr) && ISUNEVALUATED(firstchildptr))
  {
    return GETLPUNEVAL(firstchildptr);
  } else
  {
    return GETLP(firstchildptr);
  }
}

static BOOL occurslazy(
        void *state,
        Uchar *text,
        Uint textlen,
        Uchar *leftpattern,
        Uchar *rightpattern
        )
{
    Uint leftpointer, node, *nodeptr, edgelen, prefixlen;
    Uchar *lefttext, *lpatt = leftpattern, firstchar, edgechar;

    if(lpatt > rightpattern)   // check for empty word
    {
        return True;
    }
    firstchar = *lpatt;

    if(!rootevaluated)
    {
        sortByChar0();
        (void) evalrootsuccedges(suffixes,suffixes+textlen-1);
        rootevaluated = True;
    }

    CHECKROOTCHILD;

    if(ISUNEVALUATED(nodeptr))
    {
        node = NODEINDEX(nodeptr);
        evaluatenodelazy(node);
        nodeptr = streetab + node;
    }

    leftpointer = GETLP(nodeptr);
    lefttext = text + GETLP(nodeptr);
    edgelen = firstchildlp(nodeptr) - leftpointer;
    CHECKBRANCHEDGE;

    while(True)
    {
        if(lpatt > rightpattern)   // check for empty word
        {
            return True;
        }

        firstchar = *lpatt;
        nodeptr = streetab + GETFIRSTCHILD(nodeptr);

        while(True)
        {
            if(ISLEAF(nodeptr))
            {
                leftpointer = GETLP(nodeptr);
                lefttext = text + leftpointer;
                CHECKLEAFEDGE;
                if(ISRIGHTMOSTCHILD(nodeptr))
                {
                    return False;
                }
                nodeptr++;
            } else
            {
                if(ISUNEVALUATED(nodeptr))
                {
                    leftpointer = GETLPUNEVAL(nodeptr);
                } else
                {
                    leftpointer = GETLP(nodeptr);
                }
                lefttext = text + leftpointer;
                edgechar = *lefttext;
                if(edgechar == firstchar)
                {
                    break;
                }
                if(ISRIGHTMOSTCHILD(nodeptr))
                {
                    return False;
                }
                nodeptr += BRANCHWIDTH;
            }
        }
        if(ISUNEVALUATED(nodeptr))
        {
            node = NODEINDEX(nodeptr);
            evaluatenodelazy(node);
            nodeptr = streetab + node;
        }
        edgelen = firstchildlp(nodeptr) - leftpointer;
        CHECKBRANCHEDGE;
    }
}

static BOOL occurseager(/*@unused@*/ void *state,Uchar *text,
                        /*@unused@*/ Uint textlen,
                        Uchar *leftpattern,Uchar *rightpattern)
{
  Uint *nodeptr, edgelen, newleftpointer, leftpointer, prefixlen;
  Uchar *lefttext, *lpatt = leftpattern, firstchar, edgechar;

  if(lpatt > rightpattern)   // check for empty word
  {
    return True;
  }
  firstchar = *lpatt;
  CHECKROOTCHILD;
  leftpointer = GETLP(nodeptr);
  lefttext = text + leftpointer;
  nodeptr = streetab + GETFIRSTCHILD(nodeptr);
  newleftpointer = GETLP(nodeptr);
  edgelen = newleftpointer - leftpointer;
  CHECKBRANCHEDGE;
  while(True)
  {
    if(lpatt > rightpattern)   // check for empty word
    {
      return True;
    }
    firstchar = *lpatt;
    while(True)
    {
      leftpointer = GETLP(nodeptr);
      lefttext = text + leftpointer;
      if(ISLEAF(nodeptr))
      {
        CHECKLEAFEDGE;
        if(ISRIGHTMOSTCHILD(nodeptr))
        {
          return False;
        }
        nodeptr++;
      } else
      {
        edgechar = *lefttext;
        if(edgechar == firstchar)
        {
          break;
        }
        if(ISRIGHTMOSTCHILD(nodeptr))
        {
          return False;
        }
        nodeptr += BRANCHWIDTH;
      }
    }
    nodeptr = streetab + GETFIRSTCHILD(nodeptr);
    newleftpointer = GETLP(nodeptr);
    edgelen = newleftpointer - leftpointer;
    CHECKBRANCHEDGE;
  }
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
            if((Uint) (rightpattern-lpatt) == lcp(lpatt+1,rightpattern,lefttext+1,sentinel-1))\
            {\
              STOREINARRAY(resultpos,Uint,256,rootchild & ~LEAFBIT);\
              return True;\
            }\
            return False;\
          }\
          nodeptr = streetab + rootchild;\
        }

#define CHECKLEAFEDGEWITHPOS\
        if(lefttext == sentinel)\
        {\
          return False;\
        }\
        edgechar = *lefttext;\
        if(edgechar == firstchar)\
        {\
          if((Uint) (rightpattern - lpatt) == lcp(lpatt+1,rightpattern,lefttext+1,sentinel-1))\
          {\
            STOREINARRAY(resultpos,Uint,256,(Uint) (lefttext - text));\
            return True;\
          }\
          return False;\
        }

static void collectpositions(ArrayUint *resultpos,Uint *firstsucc)
{
  Uint leftpointer, *nodeptr = firstsucc;

  while(True)
  {
    if(ISLEAF(nodeptr))
    {
      leftpointer = GETLP(nodeptr);
      STOREINARRAY(resultpos,Uint,256,leftpointer);
      if(ISRIGHTMOSTCHILD(nodeptr))
      {
        break;
      }
      nodeptr++;
    } else
    {
      collectpositions(resultpos,nodeptr);
      if(ISRIGHTMOSTCHILD(nodeptr))
      {
        break;
      }
      nodeptr += BRANCHWIDTH;
    }
  }
}

#define CHECKBRANCHEDGEWITHPOS\
        prefixlen = UintConst(1)+lcp(lpatt+1,rightpattern,lefttext+1,lefttext+edgelen-1);\
        if(prefixlen == edgelen)\
        {\
          lpatt += edgelen;\
        } else\
        {\
          if(prefixlen == (Uint) (rightpattern - lpatt + 1))\
          {\
            collectpositions(resultpos,nodeptr);\
            return True;\
          }\
          return False;\
        }

BOOL occurrenceseager(void *state,Uchar *text,/*@unused@*/ Uint textlen,
                      Uchar *leftpattern,Uchar *rightpattern)
{
  Uint *nodeptr, edgelen, newleftpointer, leftpointer, prefixlen;
  Uchar *lefttext, *lpatt = leftpattern, firstchar, edgechar;
  ArrayUint *resultpos = (ArrayUint *) state;

  resultpos->nextfreeUint = 0;
  if(lpatt > rightpattern)   // check for empty word
  {
    return True;
  }
  firstchar = *lpatt;
  CHECKROOTCHILDWITHPOS;
  leftpointer = GETLP(nodeptr);
  lefttext = text + leftpointer;
  nodeptr = streetab + GETFIRSTCHILD(nodeptr);
  newleftpointer = GETLP(nodeptr);
  edgelen = newleftpointer - leftpointer;
  CHECKBRANCHEDGEWITHPOS;
  while(True)
  {
    if(lpatt > rightpattern)   // check for empty word
    {
      STOREINARRAY(resultpos,Uint,256,newleftpointer);
      return True;
    }
    firstchar = *lpatt;
    while(True)
    {
      leftpointer = GETLP(nodeptr);
      lefttext = text + leftpointer;
      if(ISLEAF(nodeptr))
      {
        CHECKLEAFEDGEWITHPOS;
        if(ISRIGHTMOSTCHILD(nodeptr))
        {
          return False;
        }
        nodeptr++;
      } else
      {
        edgechar = *lefttext;
        if(edgechar == firstchar)
        {
          break;
        }
        if(ISRIGHTMOSTCHILD(nodeptr))
        {
          return False;
        }
        nodeptr += BRANCHWIDTH;
      }
    }
    nodeptr = streetab + GETFIRSTCHILD(nodeptr);
    newleftpointer = GETLP(nodeptr);
    edgelen = newleftpointer - leftpointer;
    CHECKBRANCHEDGEWITHPOS;
  }
}

#ifdef DEBUG

static void showrootchildtab(void)
{
  Uint i;

  for(i=0; i<=UCHAR_MAX; i++)
  {
    if(rootchildtab[i] != UNDEFINEDSUCC)
    {
      if(rootchildtab[i] & LEAFBIT)
      {
        printf("#(%lu)%c-successor of root is leaf %lu\n",
               (Showuint) i,
               (char) i,
               (Showuint) (rootchildtab[i] & ~LEAFBIT));
      } else
      {
        printf("#(%lu)%c-successor of root is branch %ld\n",
                (Showuint) i,
                (char) i,
                (Showsint) rootchildtab[i]);
      }
    }
  }
  printf("#~-successor of root is leaf %lu\n",(Showuint) textlen);
}

static void showstreetab(void)
{
  Uint leftpointer, *nodeptr = streetab;

  showrootchildtab();
  while(nodeptr < nextfreeentry)
  {
    if(ISLEAF(nodeptr))
    {
      printf("#%lu: ",(Showuint) NODEINDEX(nodeptr));
      leftpointer = GETLP(nodeptr);
      printf(" Leaf %lu",(Showuint) leftpointer);
      if(ISRIGHTMOSTCHILD(nodeptr))
      {
        printf(" (last)");
      }
      nodeptr++;
    } else
    {
      printf("#%lu: ",(Showuint) NODEINDEX(nodeptr));
      leftpointer = GETLP(nodeptr);
      printf(" Branch(%lu,%lu)",(Showuint) leftpointer,
                                (Showuint) GETFIRSTCHILD(nodeptr));
      if(ISRIGHTMOSTCHILD(nodeptr))
      {
        printf(" (last)");
      }
      nodeptr += BRANCHWIDTH;
    }
    (void) putchar('\n');
  }
}

static Uint getedgelen(Uint *nodeptr)
{
  return GETLP(streetab + GETFIRSTCHILD(nodeptr)) - GETLP(nodeptr);
}

static void showsubtree(Uint *father,int indent);

static void scanedgelist(Uint firstchar,Uint *firstsucc,int indent)
{
  Uint leftpointer, edgelen, *nodeptr = firstsucc;
  Uchar *lefttext;

  while(True)
  {
    if(ISLEAF(nodeptr))
    {
      leftpointer = GETLP(nodeptr);
      lefttext = text + leftpointer;
      if(lefttext == sentinel)
      {
        if(firstchar == (Uint) (UCHAR_MAX+1))
        {
          printf("%*s~\n",indent,"");
        }
      } else
      {
        if(firstchar == *lefttext)
        {
          printf("%*s",indent,"");
          showstring(lefttext, sentinel);
          (void) putchar('\n');
        }
      }
      if(ISRIGHTMOSTCHILD(nodeptr))
      {
        break;
      }
      nodeptr++;
    } else
    {
      leftpointer = GETLP(nodeptr);
      lefttext = text + leftpointer;
      if(firstchar == *lefttext)
      {
        edgelen = getedgelen(nodeptr);
        printf("%*s",indent,"");
        showstring(lefttext,lefttext + edgelen - 1);
        (void) putchar('\n');
        showsubtree(nodeptr,indent+6);
      }
      if(ISRIGHTMOSTCHILD(nodeptr))
      {
        break;
      }
      nodeptr += BRANCHWIDTH;
    }
  }
}

static void showsubtree(Uint *father,int indent)
{
  Uint *nodeptr;
  Uchar *cptr;

  DEBUG1(5,"showsubtree(%lu)\n",(Showuint) NODEINDEX(father));
  nodeptr = streetab + GETFIRSTCHILD(father);
  for(cptr = characters; cptr < characters+alphasize; cptr++)
  {
    scanedgelist((Uint) *cptr,nodeptr,indent);
  }
  scanedgelist((Uint) (UCHAR_MAX+1),nodeptr,indent);
}

static void showtree(void)
{
  Uint leafnum, edgelen, *nodeptr, *rcptr;
  Uchar *lefttext;

  for(rcptr = rootchildtab; rcptr <= rootchildtab+UCHAR_MAX; rcptr++)
  {
    if(*rcptr != UNDEFINEDSUCC)
    {
      if(ISLEAF(rcptr))
      {
        leafnum = *rcptr & ~LEAFBIT;
        lefttext = text + leafnum;
        showstring(lefttext,sentinel);
        (void) putchar('\n');
      } else
      {
        nodeptr = streetab + *rcptr;
        lefttext = text + GETLP(nodeptr);
        edgelen = getedgelen(nodeptr);
        showstring(lefttext,lefttext + edgelen - 1);
        (void) putchar('\n');
        showsubtree(nodeptr,6);
      }
    }
  }
  printf("~\n");
}

#endif

static void showpattern(Uchar *w, Uint wlen)
{
  (void) fwrite(w,sizeof(Uchar),(size_t) wlen,stderr);
}

static void wotd_benchmark(
                BOOL evaleager,
                char *argv[],
                int argc,
                float rho,
                Uint minpat,
                Uint maxpat
            )
{
    ArrayUint resultpos;
    inittree();
    if(evaleager)
    {
        initclock();
        evaluateeager();
        DEBUGCODE(3,showstreetab());
        DEBUGCODE(3,showtree());
        FREESPACE(suffixes);
    }
    INITARRAY(&resultpos,Uint);

    if(maxpat > 0 && maxpat <= textlen && rho != 0.0)
    {
        searchpattern(
                evaleager ? occurseager : occurslazy,
                argv,
                argc,
                (void *) &resultpos,
                text,
                textlen,
                rho,
                minpat,
                maxpat,
                showpattern,
                NULL
                );
    }

    FREEARRAY(&resultpos,Uint);
    DEBUG3(2,"#maxstack=%lu %lu %lu ",
            (Showuint) maxstacksize,
            (Showuint) textlen,
            (Showuint) NODEINDEX(nextfreeentry));
    DEBUG2(2,"%lu %.2f ",(Showuint) maxwidth,(double) maxwidth/textlen);
    DEBUG2(2,"%lu %.2f\n",(Showuint) sbufferwidth,(double) sbufferwidth/textlen);
    DEBUG4(2,"#q=%lu l=%lu %lu %.2f\n",
            (Showuint) branchcount,
            (Showuint) leafcount,
            (Showuint) (branchcount*BRANCHWIDTH+leafcount),
            (double) (4*(branchcount*BRANCHWIDTH+ leafcount))/textlen);
}

void wotd(BOOL evaleager, char ***patterns_p, int npatterns)
{

    char **patterns = *patterns_p;
    ArrayUint resultpos;

    if(!evaleager) {

        initclock();
        inittree();

    } else {

        inittree();
        initclock();
        evaluateeager();
        DEBUGCODE(3,showstreetab());
        DEBUGCODE(3,showtree());
        FREESPACE(suffixes);
    }

    INITARRAY(&resultpos,Uint);

    int j;
    for(j = 0; j < npatterns; j++) {
        printf("%s\n", patterns[j]);
        search_one_pattern(
                evaleager ? occurseager : occurslazy,
                (void *) &resultpos,
                text,
                textlen,
                strlen(patterns[j]),
                patterns[j]
            );
    }

    FREEARRAY(&resultpos,Uint);
    DEBUG3(2,"#maxstack=%lu %lu %lu ",
            (Showuint) maxstacksize,
            (Showuint) textlen,
            (Showuint) NODEINDEX(nextfreeentry));
    DEBUG2(2,"%lu %.2f ",(Showuint) maxwidth,(double) maxwidth/textlen);
    DEBUG2(2,"%lu %.2f\n",(Showuint) sbufferwidth,(double) sbufferwidth/textlen);
    DEBUG4(2,"#q=%lu l=%lu %lu %.2f\n",
            (Showuint) branchcount,
            (Showuint) leafcount,
            (Showuint) (branchcount*BRANCHWIDTH+leafcount),
            (double) (4*(branchcount*BRANCHWIDTH+ leafcount))/textlen);
}

