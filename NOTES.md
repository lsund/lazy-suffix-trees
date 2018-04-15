# TODO

Read unicode specification
ask stackoverflow question on mapping unicode character to integer

# MISC

rho is trialpercentage. Number of trials is (textlen * rho)

export DEBUGLEVEL to enable debugging

some functions like allocandusespaceptr is not implemented

UTF encoding:

ASSUMPTIONS:

1. pattern file and text file are encoded in the same way


# OBSERVATIONS:

1. Only characters in the pattern file needs to be considered, due to lazy
   construction

2. The sort is based on the fact that characters are of 1 byte (a normal
   character). By preprocessing the text into long unsigned ints, it will be
   possible to represent all characters. This will however require 4 times as
   much space.

# MEMORY:

* Memory is allocated for the algorithm in one place:
  `spaceman:get_sortbuffer`. Here, one byte is allocated for each suffix.

* This space is 'chopped' into chunks by the sorting algorithm. Under the
  assumption that the first character only can be a Unsigned char, only an
  array with 258 entries is needed to manage the bounds of these chunks.

* UTF-8 requires at most 4 bytes per 'character'


# IMPLICATIONS:

1. The suffix tree needs to represent a sequence of codepoints instead of a
sequence of ascii characters.

2. Each character is an `wchar_t` instead of a Uchar. This is size 4 bytes,
   just like Uint.

3. A mapping is needed from all characters in the pattern file to a specific
   index.

4. Instead of manipulating `suffix_heads` directly, it is done through a common
   interface. These functions first retrieve the index, then manipulate
   `suffix_heads`.


## These global variables need to change

* `text`                pointer to Uchar            -> pointer to `wchar_t`
* `sentinel`            pointer to Uchar            -> pointer to `wchar_t`
* `suffixes`            pointer to Uchar pointers   -> pointer to `wchar_t` pointers
* `characters`          array of Uchar              -> hashtable of `wchar_t` -> `wchar_t`

## Functions that needs to change

* `get_characters` Assumes Uchars. The logic is OK, but both loops assume that
  counts is a static array of size 256. This needs to be a dynamic array that
  can store arbitrarily many elements.

### sort.c

* `get_count`: is fine

* `set_group_bounds`: `lower_bound` and `upper_bounds` need to be a pointer to
  `wchar_t` pointers.

CONTINUE AT INSERT SUFFIXES

