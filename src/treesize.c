
#include "types.h"
#include "globalvar.h"
#include <stdlib.h>
#include <string.h>
#include "wotd.h"
#include "debugdef.h"
#include "spacedef.h"

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

  return maxstacksize;
}
