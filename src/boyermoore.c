
#include "boyermoore.h"

Bool bmhsearch(
        Uchar *text,
        Uint textlen,
        Uchar *pattern,
        Uchar *patternright
    )
{
  Uint m, i, j, rmostocc[UCHAR_MAX+1] = {0};
  Sint k;

  m = (Uint) (patternright - pattern + 1);
  for (i=0; i<m-1; i++)
  {
    rmostocc[(Uint) pattern[i]] = i+1;
  }
  for(j = 0; j <= textlen-m; j += m-rmostocc[(Uint) text[j+m-1]])
  {
    for(k=(Sint) (m-1); k>=0 && pattern[k] == text[j+k]; k--)
      /* nothing */ ;
    if (k < 0)
    {
      return True;
      // pattern at position j+m
    }
  }
  return False;
}

