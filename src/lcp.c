#include "lcp.h"


Uint lcp(Uchar *start1,Uchar *end1,Uchar *start2,Uchar *end2)
{
  Uchar *ptr1 = start1, *ptr2 = start2;

  while(ptr1 <= end1 && ptr2 <= end2 && *ptr1 == *ptr2)
  {
    ptr1++;
    ptr2++;
  }
  return (Uint) (ptr1-start1);
}

Uint grouplcp(Uchar **left,Uchar **right)
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

