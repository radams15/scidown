#include "parser.h"

#include <assert.h>
#include <stdlib.h>

static ds_bool
is_spacer (char c)
{
  return c == ' ' || c == '\t';
}

static element*
gen_element(dyniter      start,
            dyniter      end,
            const char * name,
            ETYPE        type,
            eid          id)
{
  return element_new((edef){name, type, id}, (dynrange){start, end});
}

static ds_bool
is_section(dyniter *iter,
           element *container)
{
  assert(iter);
  if (iter->column == 0  && dyniter_at(*iter) == '#') {
    dyniter next = *iter;
    dyniter_next(&next);
    if (is_spacer(dyniter_at(next))) {
      do {
        if (!dyniter_next(&next))
          return FALSE;
        if (dyniter_at(next) == '\n')
          return FALSE;
        if (!is_spacer(dyniter_at(next)))
          return TRUE;
      } while(TRUE);
    }
  }
  return FALSE;
}

static ds_bool
is_subsection(dyniter *iter,
              element *container)
{
  assert(iter);
  if (iter->column == 0) {
    dyniter next = *iter;
    size_t i;

    for (i = 0; i < 2; i++) {
      if (dyniter_at(next) != '#' || !dyniter_next(&next))
        return FALSE;
    }

    if (is_spacer(dyniter_at(next))) {
      do {
        if (!dyniter_next(&next))
          return FALSE;
        if (dyniter_at(next) == '\n')
          return FALSE;
        if (!is_spacer(dyniter_at(next)))
          return TRUE;
      } while(TRUE);
    }
  }
  return FALSE;
}

static ds_bool
is_italic(dyniter *it,
          element *cont)
{
  if (dyniter_at(*it) == '*')
  {
    dyniter prev = *it;
    dyniter next = *it;
    dyniter_next(&next);
    if (dyniter_at(next) != '*' &&
        dyniter_at(next) != '\n' &&
        !is_spacer(dyniter_at(next))) {
      prev = next;
      while (dyniter_next(&next)) {
        if (dyniter_at(next) == '*' &&
            !is_spacer(dyniter_at(prev))) {
          return TRUE;
        }
        if (dyniter_at(next) == '\n')
          return FALSE;
        prev = next;
      }
    }
  }
  return FALSE;
}

static element*
gen_section (dyniter *iter)
{
  assert(iter);
  dyniter start = *iter;
  dyniter_end_line(iter);
  dyniter end = *iter;

  return gen_element(start, end, "Section", INLINE, SECTION);
}

static element*
gen_subsection (dyniter *iter)
{
  assert(iter);
  dyniter start = *iter;
  dyniter_end_line(iter);
  dyniter end = *iter;

  return gen_element(start, end, "Sub-section", INLINE, SUBSECTION);
}

static element*
gen_italic (dyniter *iter) {
  assert(iter);
  dyniter start = *iter;
  dyniter end = start;
  while(dyniter_next(&end)) {
    if (dyniter_at(end) == '*') {
      return gen_element(start, end, "Italic", INLINE, ITALIC);
    }
  }
  return NULL;
}


parser
default_parser ()
{
  size_t n = 3;
  pfunc * funcs = malloc(n * sizeof *funcs);

  funcs[0] = pfunc_new (SECTION, 0, is_section, gen_section);
  funcs[1] = pfunc_new (SUBSECTION, 0, is_subsection, gen_subsection);
  funcs[2] = pfunc_new (ITALIC, 0, is_italic, gen_italic);

  return (parser){funcs, n};
}
