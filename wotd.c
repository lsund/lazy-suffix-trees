//\documentclass[11pt]{article}
//\usepackage{alltt,dina4,default,command,environment}
//\newcommand{\Define}[1]{}
//\newcommand{\Cfunction}[1]{}
//\title{\textbf{Implementation of Lazy Suffix Trees:}\\\textbf{Documentation of the Source Code}}
//\author{\emph{Stefan Kurtz}}
//\begin{document}
//\maketitle
//\Ignore{

/*
  Copyright (c) 1999-2003 by Stefan Kurtz
  This is OSI Certified Open Source Software.
  Please see the file LICENSE for licensing information.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include "types.h"
#include "debugdef.h"
#include "spacedef.h"
#include "experim.h"
#include "intbits.h"
#include "args.h"
#include "arraydef.h"
#include "fhandledef.h"
#include "protodef.h"

//}

//\section{Some Basic Preprocessor Definitions}

/*
  We need four space blocks. Their names remind of the pointers, by which
  the blocks are referred to. The \texttt{stack}-block is only used for
  eager evaluation.
*/

/*
  Each branching node requires 2 unsigned integers.
  Hence the following macro. If additional information has to be stored
  for a branching node, then \texttt{BRANCHWIDTH} must be redefined
  accordingly.
*/

#define BRANCHWIDTH             UintConst(2)

/*
  The suffix tree is stored in a table \texttt{streetab}, called \(T\) in
  the paper.
  Given a pointer into \texttt{streetab}, we can determine the index the
  node is stored at.
*/

#define NODEINDEX(N)        ((Uint) ((N) - streetab))

/*
  The most significant bits in the entries of table \texttt{streetab}
  are used as marking bits. For each branching node we need 3 marking bits, and
  for each leaf we need 2 marking bits. For each leaf and each
  branching node we store 2 marking bits in their first integer: The
  \emph{leaf-bit} and the \emph{rightmost-child} bit. For
  each branching node we store 1 marking bit in the second integer,
  the \emph{unevaluated} bit.
*/

#define LEAFBIT             FIRSTBIT // leaf at this address
#define RIGHTMOSTCHILDBIT   SECONDBIT // right most child of succ list
#define UNEVALUATEDBIT      FIRSTBIT // unevaluated branching node

/*
  As a consequence, we have 30 bits to store
  \(\mathit{left}(\overline{u})\) or \(\mathit{lp}(\overline{u})\)
  for a branching node or leaf \(\overline{u}\).
  Moreover, we have 31 bits to store \(\mathit{right}(\overline{u})\) or
  \(\mathit{firstchild}(\overline{u})\) for a branching node \(\overline{u}\).
  \(\mathit{left}(\overline{u})\),
  \(\mathit{right}(\overline{u})\), and
  \(\mathit{lp}(\overline{u})\) are in the range \([0,n]\).
  \(\mathit{firstchild}(\overline{u})\) is in the range \([0,3n]\). Hence
  \(3n\leq 2^{31}-1\) must be satisfied, i.e.\
  the maximal length of the input string is \(715{,}827{,}882\).
*/

#define MAXTEXTLEN          UintConst(715827882)

/*
  The following macros test whether the corresponding bits are set.
  The argument is the pointer to the \emph{first} integer of the corresponding
  record.
*/

#define ISLEAF(P)           ((*(P)) & LEAFBIT)
#define ISRIGHTMOSTCHILD(P) ((*(P)) & RIGHTMOSTCHILDBIT)
#define ISUNEVALUATED(P)    ((*((P)+1)) & UNEVALUATEDBIT)

/*
  Given a pointer to the first index of a branching or leaf record, we obtain
  the \emph{lp}-value and the \emph{firstchild}-value by the following
  macros. There are two corresponding macros to set these values.
  Note that in \texttt{GETFIRSTCHILD} we do not have to strip the
  \emph{unevaluated}-bit, since this is not set for an unevaluated node.
  In contrast, in \texttt{SETLP} we have to take care that
  the rightmost child is maintained. For a leaf we set the \emph{lp}-value and
  the leaf bit.
*/

#define GETLP(P)            ((*(P)) & ~(LEAFBIT | RIGHTMOSTCHILDBIT))
#define GETFIRSTCHILD(P)    (*((P)+1))
#define SETLP(P,LP)         *(P) = (*(P) & RIGHTMOSTCHILDBIT) | (LP)
#define SETFIRSTCHILD(P,C)  *((P)+1) = C
#define SETLEAF(P,L)        *(P) = (L) | LEAFBIT

/*
  References are unsigned integers. An undefined reference is the following
  constant.
*/

#define UNDEFREFERENCE        (UINT_MAX)      // undefined reference

/*
  We store suffix pointers in an array \texttt{suffixes}. For each suffix
  pointer, we determine the corresponding suffix number as follows:
*/

#define SUFFIXNUMBER(L)        ((Uint) (*(L) - text))  // startposition of suffix

/*
  An unevaluated branching node \(\overline{u}\) is represented by a record
  of two integers which store two pointers \(\mathit{left}(\overline{u})\) and
  \(\mathit{right}(\overline{u})\). These pointers are relative to a pointer
  \texttt{suffixbase}. The next three macros are used for storing and
  retrieving these pointers.
*/

#define STOREBOUNDARIES(P,L,R) *(P) = (Uint) ((L) - suffixbase);\
                               *((P)+1) = ((R) - suffixbase) | UNEVALUATEDBIT
#define GETLEFTBOUNDARY(P)     (suffixbase + *(P))
#define GETRIGHTBOUNDARY(P)    (suffixbase + ((*((P)+1)) & ~UNEVALUATEDBIT))

/*
  To retrieve the \emph{lp}-value of an unevaluated node, we first retrieve
  the left boundary. This gives a pointer into the array \texttt{suffixes}.
  From the pointer we can retrieve the corresponding \emph{lp}-value.
*/

#define GETLPUNEVAL(N) SUFFIXNUMBER(GETLEFTBOUNDARY(N))

//\section{Declaration of Global Variables}

Uchar *text,                   // points to input string \(t\) of length \(n\)
      *sentinel,               // points to \(t[n]\) which is undefined
      characters[UCHAR_MAX+1], // characters in \(t\) in alphabetical order
      **suffixes,              // array of pointers to suffixes of \(t\)
      **suffixbase,            // pointers into suffixes are w.r.t.\ this var
      **sbuffer,               // buffer to sort suffixes in \texttt{sortByChar}
      **sbufferspace = NULL,   // space to be used by \texttt{sbuffer}
      **bound[UCHAR_MAX+1];    // pointers into \texttt{sbuffer} while sorting

Uint  textlen,                 // length of \(t\)
      alphasize,               // size of alphabet \(\Sigma\)
      alphaindex[UCHAR_MAX+1], // index of characters in \(\Sigma\)
      occurrence[UCHAR_MAX+1], // number of occurrences of character
      *streetab = NULL,        // table to hold suffix tree representation
      streetabsize,            // number of integers in \texttt{streetab} allocated
      *nextfreeentry,          // pointer to next unused element in \texttt{streetab}
      sbufferwidth,            // number of elements in \texttt{sbufferspace}
      maxsbufferwidth,         // maximal number of elements in \texttt{sbufferspace}
      suffixessize,            // number of unprocessed suffixes (for eager)
      maxunusedsuffixes,       // when reached, then move and halve space for suffixes
      rootchildtab[UCHAR_MAX+1]; // constant time access to successors of \emph{root}

/*
  \texttt{rootchildtab} is initialized in \texttt{evalrootsuccedges}.
  For each ASCII-character \(a\) the following holds:
  \begin{itemize}
  \item
  If there is no \(a\)-successor, then
  \(rootchildtab[a]=\texttt{UNDEFINEDSUCC}\), where the latter constant is
  defined below.
  \item
  If the \(a\)-successor leads to the leaf stored at index \(i\), then
  \(rootchildtab[a]=i\texttt{ | LEAFBIT}\).
  \item
  If the \(a\)-successor leads to the branching node stored at index \(i\),
  then \(rootchildtab[a]=i\).
  \end{itemize}
*/

#define UNDEFINEDSUCC  (UINT_MAX)    // undefined successor

BOOL  rootevaluated;   // flag indicating that the root has been evaluated

//\Ignore{

#ifdef DEBUG
Uint lastrootchild, maxstacksize, maxwidth, branchcount, leafcount;

void showstring(Uchar *left,Uchar *right)
{
  Uchar *ptr;
  for(ptr = left; ptr <= right; ptr++)
  {
    if(ptr == sentinel)
    {
      (void) putchar('~');
    } else
    {
      (void) putchar((Fputcfirstargtype) *ptr);
    }
  }
}
#endif

//}

//\section{Space Management}

/*
  The following function does the space management for the array
  \texttt{sbuffer} in the case of eager evaluation. We always know that
  all suffixes stored between \texttt{suffixes[0]} and
  \texttt{suffixes[q-1]}, where \(q=\texttt{left-suffixes}\), are already
  processed. If this range is
  large enough to hold all suffixes between \texttt{left} and \texttt{right},
  then we can use the space to the left of \texttt{left} as space for
  \texttt{sbuffer}. If this range is not large enough, we test if the
  number of elements in \texttt{sbufferspace} is large enough. If not, then
  we enlarge it accordingly, and return \texttt{sbufferspace}.
*/

static Uchar **getsbufferspaceeager(Uchar **left,Uchar **right)
{
  Uint width = (Uint) (right-left+1);

  if(width > (Uint) (left-suffixes))
  {
    DEBUG2(3,"#bucket: left=%lu width=%lu\n",(Showuint) (left-suffixes),
                                             (Showuint) width);
    if(width > sbufferwidth)
    {
      sbufferwidth = width;
      ALLOCASSIGNSPACE(sbufferspace,sbufferspace,Uchar *,sbufferwidth);
    }
    return sbufferspace;
  }
  return left - width;
}

/*
  The following function does the space management for the array
  \texttt{sbuffer} in the case of lazy evaluation. If the width of the
  \texttt{sbufferspace} becomes smaller than some maximal width, then
  we reduce the size of this array. If the array is too small, then
  we enlarge it accordingly.
*/

static Uchar **getsbufferspacelazy(Uchar **left,Uchar **right)
{
  Uint width = (Uint) (right-left+1);

  if(sbufferwidth > maxsbufferwidth && maxsbufferwidth > width)
  {
    sbufferwidth = maxsbufferwidth;
    ALLOCASSIGNSPACE(sbufferspace,sbufferspace,Uchar *,sbufferwidth);
  } else
  {
    if(width > sbufferwidth)
    {
      sbufferwidth = width;
      ALLOCASSIGNSPACE(sbufferspace,sbufferspace,Uchar *,sbufferwidth);
    }
  }
  return sbufferspace;
}

/*
  Since we have a virtual sentinel character which is different from
  all possible 256 characters of the ASCII-alphabet, a branching node
  can have \texttt{UCHAR\_MAX+2=257} successor nodes. The sentinel edge
  leads to a leaf, but each of the other edges can lead to a branching
  node. Hence, to evaluate a branching node, we have to enlarge
  the size of \texttt{streetab} by at least \texttt{MAXSUCCSPACE}
  integers. This is done by the function \texttt{allocstreetab},
  when \texttt{nextfreeentry} reaches the end of the block currently
  allocated for \texttt{streetab}.
*/

#define MAXSUCCSPACE            (BRANCHWIDTH * (UCHAR_MAX+1) + 1)

static void allocstreetab(void)
{
  Uint tmpindex = NODEINDEX(nextfreeentry);
  if(tmpindex >= streetabsize)
  {
    streetabsize += (textlen/10);
    ALLOCASSIGNSPACE(streetab,streetab,Uint,streetabsize + MAXSUCCSPACE);
    // update necessary, since streetab may have been moved.
    nextfreeentry = streetab + tmpindex;
  }
}

//\section{Sorting and Grouping Suffixes}

/*
  To sort the suffixes of one group according to their first character,
  we use the function \texttt{sortByChar} which implements a counting sort
  algorithm. It takes as arguments the left and right bound \texttt{left} and
  \texttt{right} of the subarray representing the group to be sorted, and
  a commong prefix of all suffixes which has not been dropped from the
  beginning of the suffix. In the first loop, this prefix is dropped
  from each of the suffixes. Note that a bucket contains the suffixes
  in the order of their length. This stability is achieved by inserting
  them from right to left. Note that there is no loop over all the
  characters in the alphabet. Instead to determine the boundaries
  of the subgroups, one loops over all suffixes of the group. For small
  groups this is faster.
*/

static void sortByChar(Uchar **left,Uchar **right,Uint prefixlen)
{
  Uchar **i, **j, **nextFree = sbuffer;
  Uint a;

  DEBUGCODE(1,if((Uint) (right-left+1) > maxwidth) { maxwidth = (Uint) (right-left+1);});
  if(*right + prefixlen == sentinel)  // shortest suffix is sentinel: skip
  {
    *right += prefixlen;
    right--;
  }
  for(i=left; i<=right; i++) // determine size for each group
  {
    *i += prefixlen;         // drop the common prefix
    occurrence[(Uint) **i]++;
  }
  for(i=left; i<=right; i++) // determine right bound for each group
  {
    a = (Uint) **i;
    if(occurrence[a] > 0)
    {
      bound[a] = nextFree+occurrence[a]-1;
      nextFree = bound[a]+1;
      occurrence[a] = 0;
    }
  }
  for(i=right; i>=left; i--) // insert suffixes into buffer
  {
    *(bound[(Uint) **i]--) = *i;
  }
  for(i=left,j=sbuffer; i<=right; i++,j++) // copy grouped suffixes back
  {
    *i = *j;
  }
}

/*
  We could also call \texttt{sortByChar} to determine
  the groups for all suffixes of the input string. This would however
  require \texttt{sbuffer} to be of size \texttt{textlen}. To save this space
  and to speed up the sorting, we have a special version of
  \texttt{sortByChar}. It scans all suffixes from left to right,
  determines the size of each group, and then directly sorts all suffixes
  into the array \texttt{suffixes}.
*/

static void sortByChar0(void)
{
  Uchar *cptr, **nextFree = suffixes;
  Uint a;

  for(cptr=text; cptr < text+textlen; cptr++) // determine size for each group
  {
    occurrence[(Uint) *cptr]++;
  }
  for(cptr=characters; cptr < characters+alphasize; cptr++)
  {
    a = (Uint) *cptr;
    bound[a] = nextFree+occurrence[a]-1;
    nextFree = bound[a]+1;
    occurrence[a] = 0;
  }
  for(cptr=text+textlen-1; cptr>=text; cptr--) // insert suffixes into array
  {
   *(bound[(Uint) *cptr]--) = cptr;
  }
  suffixes[textlen] = sentinel;  // suffix \$ is the largest suffix
}

/*
  To determine the length of the longest common prefix of some
  suffixes, we implement the function \texttt{grouplcp}. It loops
  over all \(j=1,2,\ldots\), and tests for all suffixes if the
  \(j\)th characters are all identical. As soon as a character
  is found which is not identical, \texttt{grouplcp} returns the
  length \(j\) of the longest common prefix. Note that we already known
  that the first character of all suffixes in the group are identical,
  so it is correct to start with \(j=1\). As soon as the \(j\)th character
  of the smallest suffix of the group is the sentinel character,
  the procedure stops and has found the longest prefix.
*/

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

//\section{The Evaluation Process}

/*
  The following function evaluates all edges outgoing from an unevaluated
  node \(\overline{u}\). The suffixes in the subtree below \(\overline{u}\)
  are found between the pointers \(\mathit{left}=\mathit{left}(\overline{u})\)
  and \(\mathit{right}=\mathit{right}(\overline{u})\).
  \texttt{evalsuccedges} returns a reference to the first unevaluated
  branching node, if such a node exists. Otherwise it returns
  \texttt{UNDEFREFERENCE}. \texttt{previousnode} always refers to the left
  brother of the current node, if such a node exist.
*/

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
    if(r > l) // create branching node
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

/*
  The function \texttt{evalrootsuccedges} evaluates all edges outgoing from the
  \(root\).  It is a specialization of the previous function, and it
  additionally initializes the array \texttt{rootchildtab} appropriately.
  We do not show the code here.
*/

//\Ignore{

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

//}

/*
  The following function \texttt{evaluatenodeeager} evaluates the node
  referenced by \texttt{node}. It is used for eager evaluation.
  \texttt{evaluatenodeeager} extracts the left and right boundaries of the group
  of suffixes which end in the corresponding subtree. Then the \emph{lp}-value
  and \emph{firstchild}-value of that node are set.
  Subsequently the longest common prefix of the suffixes
  of that group is determined, and the group is divided into subgroups by
  sorting these according to the first character of the remaining suffixes.
  Then the edges outgoing from \texttt{node} are constructed.

  The middle part of the function deals with the space management:
  In eager evaluation, the suffix pointers in the array \texttt{suffixes}
  are processed from left to right.
  In other words, if we have processed the suffixes up to the suffix stored
  at, say \texttt{suffixes[q]}, then we do not need the entries at indices
  \(0...q\). Hence, if \(q\) reaches a certain threshold, we move all
  suffix pointers at positions larger than \(q\) exactly \(q\) positions
  to the left, and reduce the size of the array \texttt{suffixes} to the
  number of the remaining suffixes. More precisely, we approximately
  halve the size of the array \texttt{suffixes}.
  To have valid pointers into the array \texttt{suffixes}, we let all
  suffix pointers be relative to the pointer \texttt{suffixbase}. This is
  decremented by \(q\) as well. \texttt{evaluatenodeeager} returns a reference
  to the next node to be processed in eager evaluation.
*/

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
    ALLOCASSIGNSPACE(suffixes,suffixes,Uchar *,suffixessize);
    suffixbase = suffixes - (tmpdiff + unusedsuffixes);
    left = suffixes;
    right = suffixes + width - 1;
  }
  sbuffer = getsbufferspaceeager(left,right);
  prefixlen = grouplcp(left,right);
  sortByChar(left,right,prefixlen);
  return evalsuccedges(left,right);
}

/*
  The following function is identical to \texttt{evaluatenodeeager}, except
  that it does not halve the size of the table suffixes. This is not
  possible for lazy evaluation, since the suffixes are not processed
  from left to right, but in an order determined by the traversal of
  the unevaluated suffix tree. Unlike \texttt{evaluatenodeeager}, the
  function does not return anything. \texttt{evaluatenodelazy} is
  used for lazy evaluation.
*/

//\Ignore{

#ifndef ONLYCOUNT

//}

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

#endif

/*
  Given a reference to a branching node, the following function
  returns a reference to the next branching node to the right.
  If there is no such branching node, then \texttt{getnextbranch} returns
  \texttt{UNDEFREFERENCE}.
*/

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

/*
  For the eager evaluation of the suffix tree, we evaluate the nodes in
  depth first and left to right order. To do so, we facilitate a stack,
  which stores references to the branching nodes still to be evaluated.
  The stack is implemented by an array, which is enlarged in units
  of 100 elements, if necessary.
*/

#define NOTSTACKEMPTY         (stacktop > 0)
#define PUSHNODE(N)\
        if(stacktop >= stackalloc)\
        {\
          stackalloc += 100;\
          ALLOCASSIGNSPACE(stack,stack,Uint,stackalloc);\
        }\
        DEBUGCODE(1,if(stacktop > maxstacksize) { maxstacksize = stacktop;});\
        NOTSUPPOSEDTOBENULL(stack);\
        stack[stacktop++] = N

#define POPNODE(N)\
        N = stack[--stacktop]

/*
  The following function evaluates all nodes of the suffix tree in
  a depth first left to right strategy. This is achieved by using a
  stack (rather than recursively, which would use much more memory):
  In the outer loop the elements are popped from the stack.
  In the inner loop we always test if the current node has
  a right brother which is branching. If so, the right brother is pushed, and
  we proceed with the first child of the current node which is branching.
  This is of course unevaluated.
*/

static void evaluateeager(void)
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

//\section{Initializing the Data Structures}

/*
  The following function allocates and initializes the data structures.
  In particular it inserts the root with depth 0, and the appropriate
  boundaries. The latter are however not necessary, since if the
  depth is 0, we know them anyway.
*/

static void inittree(void)
{
  Uint i;

  DEBUGCODE(1,maxstacksize=maxwidth=branchcount=leafcount=0);
  getUchars(text,textlen,characters,&alphasize);
  sentinel = text+textlen;
  streetabsize = BRANCHWIDTH;
  ALLOCASSIGNSPACE(streetab,streetab,Uint,streetabsize + MAXSUCCSPACE);
  nextfreeentry = streetab;
  suffixessize = textlen+1;
  maxunusedsuffixes = suffixessize >> 1;
  ALLOCASSIGNSPACE(suffixes,NULL,Uchar *,suffixessize);
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

//\section{Searching for Patterns}

/*
  The following function computes the length of the longest common prefix
  of two strings given by pairs of pointers to the first and last
  characters.
*/

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

/*
  We have two different functions to test whether some string occurs
  in the suffix tree or not. One handles the case that the suffix tree
  is evaluated lazily, the other handles the case that the suffix tree
  has been completely evaluated before starting the pattern search.
  We only give the code for the former. Both functions share a large
  amount of code. We therefore use three macros:
  \texttt{CHECKROOTCHILD} tests for the given character \texttt{firstchar},
  whether there is a \texttt{firstchar}-edge outgoing from the root.
  If this edge leads to a leaf, we test if the remaining pattern suffix
  is a prefix of the label of the leaf edge. If the edge leads to
  a branching node, then we continue with this.
*/

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

/*
  The following macro tests whether the remaining suffix of the pattern
  is a prefix of the label of the current leaf edge.
*/

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

/*
  The following macro tests whether the remaining suffix of the pattern
  is a prefix of the label of the current edge to a branching node.
*/

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

/*
  The following function retrieves the \emph{lp}-value of the first child
  for a given branching node. In case the firstchild is an unevaluated
  branching node, the returned value is delivered by the macro
  \texttt{GETLPUNEVAL}. In case the firstchild is a leaf or
  an evaluated node, the returned value can directly be retrieved,
  given the reference to the first child.
*/

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

/*
  The function \texttt{occurslazy} takes a void pointer,
  as well as a pointer to the input string and its length as an argument.
  These three parameters are not used here, but we keep them since
  the function is passed to the function \texttt{searchpattern} (defined
  in the library module \texttt{searchpat}).
  The last two arguments are of interest. They refer to the first and the last
  character of the pattern to be searched. The function works in
  two loops. The outer loop scans down the suffix tree, and
  the inner loop scans the list of successors of a given node.
  For the root there is no inner loop, since we access
  table \texttt{rootchildtab}. Note that before following an edge
  to a branching node, we first evaluate that node, if it is not
  already evaluated.
*/

//\Ignore{

#ifndef ONLYCOUNT

//}

static BOOL occurslazy(/*@unused@*/ void *state,Uchar *text,Uint textlen,
                       Uchar *leftpattern,Uchar *rightpattern)
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

/*
  The function \texttt{occurseager} has the same interface as
  \texttt{occurslazy}.
  It is slightly simpler, due to the fact that it assumes that the
  suffix tree has been completely evaluated. We do not show the code
  here.
*/

//\Ignore{

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

#endif

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
          showstring(lefttext,sentinel);
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

//}

//\section{Putting it all together}

/*
  \texttt{wotd} implements the \emph{wotd}-algorithm and calls the functions
  to search for patterns. If the first argument
  is true, then the suffix tree is evaluated with the eager strategy.
  Otherwise, the suffix tree is evaluated with the lazy strategy.
  Note that in this case the evaluation of nodes is done in the function
  \texttt{occurslazy}.
*/

//\Ignore{

#ifndef ONLYCOUNT

//}

static void showpattern(/*@unused@*/ void *info,Uchar *w,Uint wlen)
{
  (void) fwrite(w,sizeof(Uchar),(size_t) wlen,stderr);
}

static void wotd(BOOL evaleager,char *argv[],int argc,float rho,
                 Uint minpat,Uint maxpat)
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
    searchpattern(evaleager ? occurseager
                            : occurslazy,argv,argc,
                  (void *) &resultpos,text,textlen,rho,minpat,maxpat,
                  showpattern,NULL);
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

//\Ignore{

#endif

//}

//\end{document}

#ifdef ONLYCOUNT
Uint wotdtreesize(Uchar *textarg,Uint textlenarg,Uchar *alphabet,
                  Uint alphasizearg)
{
  text = textarg;
  textlen = textlenarg;
  alphasize = alphasizearg;
  memcpy(characters,alphabet,sizeof(Uchar) * alphasize);
  inittree();
  evaluateeager();
  DEBUGCODE(3,showstreetab());
  DEBUGCODE(3,showtree());
  FREESPACE(suffixes);

#ifndef NOSPACEBOOKKEEPING
  checkspaceleak();
#endif
  return maxstacksize;
}
#else
int main(int argc,char *argv[])
{
  float rho, readfloat;
  Uint minpat, maxpat;
  char *filename;
  BOOL evaleager;
  Scaninteger readint;

  CHECKARGNUM(6,"(-lazy|-eager) rho minpat maxpat filename");
  DEBUGLEVELSET;
  if(strcmp(argv[1],"-lazy") != 0 && strcmp(argv[1],"-eager") != 0)
  {
    fprintf(stderr,"Illegal option \"%s\"\n",argv[1]);
    exit(EXIT_FAILURE);
  }
  if(strcmp(argv[1],"-eager") == 0)
  {
    evaleager = True;
  } else
  {
    evaleager = False;
  }

  PARSEFLOATARG(argv[2]);
  rho = readfloat;
  PARSEINTARG(argv[3]);
  minpat = (Uint) readint;
  PARSEINTARG(argv[4]);
  maxpat = (Uint) readint;
  filename = argv[5];
  text = (Uchar *) file2String(filename,&textlen);
  if(text == NULL)
  {
    fprintf(stderr,"%s: Cannot open file %s\n",argv[0],filename);
    exit(EXIT_FAILURE);
  }
  if(textlen > MAXTEXTLEN)
  {
    fprintf(stderr,"Sorry, textlen = %lu is larger than maximal textlen = %lu\n",
                   (Showuint) textlen,(Showuint) MAXTEXTLEN);
    exit(EXIT_FAILURE);
  }
  if(!evaleager)
  {
    initclock();
  }
  wotd(evaleager,argv,argc,rho,minpat,maxpat);
  freetextspace(text,textlen);
  if(evaleager)
  {
    SHOWTIME("wotdeager");
    SHOWSPACE("wotdeager");
  } else
  {
    SHOWTIME("wotdlazy");
    SHOWSPACE("wotdlazy");
  }
#ifndef NOSPACEBOOKKEEPING
  checkspaceleak();
#endif
  return EXIT_SUCCESS;
}
#endif
