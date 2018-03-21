
#include "util.h"

// Print a string to stdout
void showstring(Uchar *left, Uchar *right)
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


