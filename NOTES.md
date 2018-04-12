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

The suffix tree needs to represent a sequence of codepoints instead of a
sequence of ascii characters.

Each character needs 4 bytes of space instead of 1 byte of space.

## These global variables need to change

* `text`        pointer to Uchar            -> pointer to Uint
* `sentinel`    pointer to Uchar            -> pointer to Uint
* `suffixes`    pointer to Uchar pointers   -> pointer to Uint pointers
* `characters`  array of Uchar              -> hashtable of Uint -> Uint

## Functions that needs to change

* `get_characters` Assumes Uchars. The logic is OK, but both loops assume that
  counts is a static array of size 256. This needs to be a dynamic array that
  can store arbitrarily many elements.

## These structures need to be a linked list rather than a static array

* occurence - counts the number of occurences of all characters



