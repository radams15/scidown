#ifndef __PARSER_H__
#define __PARSER_H__

#include "ast.h"

struct pfunc {
  eid        e_id;
  int        priority;
  ds_bool  (*at)    (dyniter *it,
                     element *container);
  element* (*parse) (dyniter *it);
} typedef pfunc;

struct parser {
  pfunc     *funcs;
  size_t     n;
} typedef parser;


typedef   element    document;


pfunc    pfunc_new       (eid       id,
                          int       priority,
                          ds_bool  (*at)    (dyniter *it,
                                             element *container),
                          element* (*parse) (dyniter *it));

void      pfunc_free     (pfunc     *el);

document* root           (dyniter   *iter);
element*  text_block     (dyniter    from,
                          dyniter    to);

parser    default_parser ();

document* parse          (dynstr    *doc,
                          parser     aparser);

#endif
