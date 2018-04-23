# MISC

Can assume that file and text are encoded the same way.

# Observations:

1. Only characters in the pattern file needs to be considered, due to lazy
   construction

2. The sort is based on the fact that characters are of 1 byte (a normal
   character). By preprocessing the text into long unsigned ints, it will be
   possible to represent all characters. This will however require 4 times as
   much space.

# Memory:

* Memory is allocated for the algorithm in one place:
  `spaceman:get_sortbuffer`. Here, one byte is allocated for each suffix.

* This space is 'chopped' into chunks by the sorting algorithm. Under the
  assumption that the first character only can be a Unsigned char, only an
  array with 258 entries is needed to manage the bounds of these chunks.

* UTF-8 requires at most 4 bytes per 'character'

# Implications:

1. The suffix tree needs to represent a sequence of codepoints instead of a
sequence of ascii characters.

2. Each character is an `wchar_t` instead of a Uchar. This is size 4 bytes,
   just like Uint.

3. A mapping is needed from all characters in the pattern file to a specific
   index.

4. Instead of manipulating `suffix_heads` directly, it is done through a common
   interface. These functions first retrieve the index, then manipulate
   `suffix_heads`.

# Measurements taken

Change text from `Uchar` -> `Wchar`, ie 4 bytes instead of one. This implies
changes in almost all functions and datastructures.

# The evaluation code

## Notation

`l(v)` = the leaf set for `v`. The leaves are ordered by a total order on their
number.

Let `u -> uv` be a vertex. Then the left pointer `lp(uv) = min(l(uv)) + |u|`.

## Remaining suffixes.

* Let `u` be a string in the tree.. The subtree below `u` is represented by all
  suffixes which has `u` as prefix. Let these be called the _remaining suffixes_
  of the vertex `u`.

* To evaluate `u`, we need access to its remaining suffixes.

TODO: Why does this exists as an interval in suffixes?
* The global array `suffixes` points into the text and the remaining suffixes of
  `u` exists as a interval in `suffixes`.

* For each unevaluated node, there is an interval in suffixes which stores
  poniters to
