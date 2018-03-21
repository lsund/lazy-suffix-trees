
#include "counting_sort.h"

Uchar **bound[UCHAR_MAX + 1];

void sortByChar(Uchar **left,Uchar **right,Uint prefixlen)
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

void sortByChar0(void)
{
  Uchar *cptr, **nextFree = suffixes;
  Uint a;

  for(cptr = text; cptr < text+textlen; cptr++) // determine size for each group
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

