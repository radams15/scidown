#include "parser.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

pfunc
pfunc_new  (eid       id,
            int       priority,
            ds_bool  (*at)    (dyniter *it,
                               element *container),
            element* (*parse) (dyniter *it))
{
  pfunc res;
  res.e_id = id;
  res.at = at;
  res.parse = parse;

  return res;
}



document*
root (dyniter *it)
{
  edef definition = {"Root", BLOCK, ROOT};

  dyniter start = *it;
  dyniter *end = dyniter_copy(it);
  dyniter_goto(end, end->__src__->size-1);
  dynrange range = {start, *end};
  return element_new(definition, range);
}
element*
text_block (dyniter from,
            dyniter to)
{
  return element_new((edef){"Text", BLOCK, TEXT}, (dynrange){from, to});
}

document*
parse          (dynstr    *doc,
                parser     p)
{
  dyniter  it = {0, 0, 0, doc};
  document *base = root(&it);
  element  *current = base;
  dyniter  last = it;
  size_t   i;
  do {
    for (i = 0; i < p.n; i++) {
      if (p.funcs[i].at(&it, current)) {
        element * next = p.funcs[i].parse(&it);
        if (next->range.start.i > last.i+1) {
          element * t = text_block(last, next->range.start);
          element_append(current, t);
        }
        element_append(current, next);
        last = next->range.end;
        dyniter_next(&last);
      }
    }
  } while (dyniter_next(&it));
  if (last.i != it.i) {
     element * t = text_block(last, it);
     element_append(current, t);
  }
  return base;
}
