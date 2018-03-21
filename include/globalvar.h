#include "types.h"

Uchar *text,                   // points to input string \(t\) of length \(n\)
      *patterns,
      *sentinel,               // points to \(t[n]\) which is undefined
      characters[UCHAR_MAX+1], // characters in \(t\) in alphabetical order
      **suffixes,              // array of pointers to suffixes of \(t\)
      **suffixbase,            // pointers into suffixes are w.r.t.\ this var
      **sbuffer,               // buffer to sort suffixes in \texttt{sortByChar}
      **sbufferspace = NULL,   // space to be used by \texttt{sbuffer}
      **bound[UCHAR_MAX+1];    // pointers into \texttt{sbuffer} while sorting

Uint  textlen,                 // length of \(t\)
      patternslen,
      maxstacksize,
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


#define UNDEFINEDSUCC  (UINT_MAX)    // undefined successor

BOOL  rootevaluated;   // flag indicating that the root has been evaluated

