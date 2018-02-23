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
  return element_new(definition, range, range);
}
element*
text_block (dyniter from,
            dyniter to)
{
  dynrange r = (dynrange){from, to};
  return element_new((edef){"Text", BLOCK, TEXT}, r, r);
}

document*
parse          (dynstr    *doc,
                parser     p)
{
  dyniter  it = {0, 0, 0, doc};
  document *base = root(&it);
  parse_block(doc, base, p);
  return base;
}


void
parse_block    (dynstr    *doc,
                element   *root,
                parser    p)
{
  size_t  end  = root->inner.end.i;
  size_t  i;
  dyniter it = root->inner.start;
  dyniter last = it;
  do {
    for (i = 0; i < p.n; i++) {
      element * next;
      if (p.funcs[i].at(&it, root)) {
        next = p.funcs[i].parse(&it);
        if (next) {
          if (next->range.start.i > last.i+1) {
            dyniter to = next->range.start;
            dyniter_prev(&to);
            element * t = text_block(last, to);
            element_append(root, t);
          }
          parse_block(doc, next, p);
          element_append(root, next);
          last = next->range.end;
          it = last;
          dyniter_next(&last);
        }
      }
    }
  } while (it.i < end && dyniter_next(&it));
  if (last.i != it.i) {
     element * t = text_block(last, it);
     element_append(root, t);
  }
}
