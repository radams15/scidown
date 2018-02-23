#include "parser.h"

#include <assert.h>
#include <stdlib.h>

static ds_bool
is_spacer (char c)
{
  return c == ' ' || c == '\t';
}

static element*
gen_element(dynrange     outer,
            dynrange     inner,
            const char * name,
            ETYPE        type,
            eid          id)
{
  return element_new((edef){name, type, id}, outer, inner);
}

static element*
gen_sin_element(dyniter      start,
                dyniter      end,
                const char * name,
                ETYPE        type,
                eid          id)
{
  dynrange r = {start, end};
  return element_new((edef){name, type, id}, r, r);
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

static ds_bool
is_bold(dyniter *it,
          element *cont)
{
  dyniter nt = *it;
  if (!dyniter_next(&nt))
    return FALSE;
  if (dyniter_at(*it) == '*' && dyniter_at(nt) == '*')
  {
    dyniter prev = nt;
    dyniter next = nt;
    dyniter_next(&next);
    if (dyniter_at(next) != '\n' &&
        !is_spacer(dyniter_at(next))) {
      dyniter_next(&next);
      prev = next;
      while (dyniter_next(&next)) {
        if (dyniter_at(next) == '*' &&
            dyniter_at(prev) == '*') {
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


static void
skip_char (dyniter *iter,
           char     c)
{
    while (dyniter_next(iter)) {
      if (dyniter_at(*iter) != c)
        break;
    }
}

static element*
gen_h_title (dyniter *iter)
{
  skip_char(iter, '#');
  skip_char(iter, ' ');
  dyniter start = *iter;
  dyniter_end_line(iter);
  dyniter end = *iter;
  dyniter_prev(&end);
  return gen_sin_element(start,
                         end,
                         "H-Title",
                         INLINE,
                         HTITLE);
}

static element*
gen_section (dyniter *iter)
{
  assert(iter);
  dyniter start = *iter;
  element * title = gen_h_title(iter);
  dyniter_next(iter);
  dyniter inner = *iter;
  dyniter end = *iter;
  while (dyniter_next(&end)) {
    if (is_section(&end, NULL)){
      dyniter_prev(&end);
      break;
    }
  }
  element * e= gen_element((dynrange) {start, end},
                           (dynrange) {inner, end},
                           "Section", BLOCK, SECTION);
  element_append(e, title);
  return e;
}

static element*
gen_subsection (dyniter *iter)
{
  assert(iter);
  dyniter start = *iter;
  element * title = gen_h_title(iter);
  dyniter_next(iter);
  dyniter inner = *iter;
  dyniter end = *iter;
  while (dyniter_next(&end)) {
    if (is_section(&end, NULL) || is_subsection(&end, NULL)){
      dyniter_prev(&end);
      break;
    }
  }
  element * e = gen_element((dynrange) {start, end},
                            (dynrange) {inner, end},
                            "Sub-section", BLOCK, SUBSECTION);
  element_append(e, title);
  return e;
}

static element*
gen_italic (dyniter *iter) {
  assert(iter);
  dyniter start = *iter;
  dyniter end = start;
  while(dyniter_next(&end)) {
    if (dyniter_at(end) == '*') {
      dyniter next =end;
      dyniter_next(&next);
      char n = dyniter_at(next);
      if (n == '\n' || n == '*' || is_spacer(n)) {
        dyniter s = start;
        dyniter_next (&s);
        dyniter e = end;
        dyniter_prev (&e);
        return gen_element((dynrange) {start, end},
                           (dynrange) {s, e},
                           "Italic", INLINE, ITALIC);
      }
    }
  }
  return NULL;
}


static element*
gen_bold (dyniter *iter) {
  assert(iter);
  dyniter start = *iter;
  dyniter end = start;
  dyniter_skip(&end, 2);
  dyniter prev = end;
  dyniter next;
  while(dyniter_next(&end)) {
    next = end;
    if (!dyniter_next(&next) || (dyniter_at(end) == '*' && dyniter_at(prev)=='*'
                                 && is_spacer(dyniter_at(next)))) {
      dyniter s = start;
      dyniter_next (&s);
      dyniter_next (&s);
      dyniter e = end;
      dyniter_prev (&e);
      dyniter_prev (&e);
      return gen_element((dynrange) {start, end},
                         (dynrange) {s, e},
                         "Bold", INLINE, BOLD);
    }
    prev = end;
  }
  return NULL;
}

parser
default_parser ()
{
  size_t n = 4;
  pfunc * funcs = malloc(n * sizeof *funcs);

  funcs[0] = pfunc_new (SECTION, 0, is_section, gen_section);
  funcs[1] = pfunc_new (SUBSECTION, 0, is_subsection, gen_subsection);

  funcs[2] = pfunc_new (ITALIC, 0, is_italic, gen_italic);
  funcs[3] = pfunc_new (BOLD, 0, is_bold, gen_bold);
  return (parser){funcs, n};
}
