#ifndef EVAL_H
#define EVAL_H

#include "text.h"
#include "types.h"
#include "config.h"
#include "spaceman.h"
#include "lcp.h"
#include "sort.h"
#include "stree.h"
#include "insert.h"

// Evaluate the root by determining and inserting the appropriate edges under
// it.
void eval_root();

// Evaluate an inner vertex (if unevaluated) by determining and inserting the
// appropriate edges under it. Does nothing if the vertex is already evaluated.
void eval_if_uneval(VertexP *v);

#endif
