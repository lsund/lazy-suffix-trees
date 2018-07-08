#ifndef CONFIG_H
#define CONFIG_H

// Number of bits in an unsigned integer, depending on architecture
#define INT_SIZE                 64

// The amount of memory allocated for the vertex table
#define VERTEX_ALLOC            100000000 // 100 MB

// Maximum length of a single pattern
#define MAX_PATTERNLEN          100000

// The maximum number of patterns to be queried
#define MAX_PATTERNS            350000

// The maximum number of distinct characters allowed in the text.
#define MAX_CHARS               10000

#define N_RAND_PATTERNS         10000
#define RAND_MINLEN             10
#define RAND_MAXLEN             20

// The matching patterns will be logged to this file
#define PATTERN_OUT             "patterns.txt"

#endif
