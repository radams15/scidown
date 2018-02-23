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
is_header(dyniter i,
          size_t  l)
{
  if (i.column == 0){
    size_t j = 0;
    do {
      char c = dyniter_at(i);
      if (c == '#'){
        j++;
        if (j > l)
          return FALSE;
      } else if (is_spacer(c)) {
        if (j == l) {
          do {
            c = dyniter_at(i);
            if (c == '\n')
              return FALSE;
            else if (!is_spacer(c))
              return TRUE;
          } while (dyniter_next(&i));
        }
        return FALSE;
      } else
        return FALSE;
    } while (dyniter_next(&i));
  }
  return FALSE;
}

static ds_bool
is_section (dyniter *iter,
            element *container)
{
  assert(iter);

  return is_header(*iter, 1);
}

static ds_bool
is_subsection (dyniter *iter,
               element *container)
{
  assert(iter);
  return is_header(*iter, 2);
}

static ds_bool
is_subsubsection (dyniter *iter,
                  element *element)
{
  assert(iter);
  return is_header(*iter, 3);
}

static ds_bool
is_paragraph (dyniter *iter,
              element *element)
{
  assert(iter);
  return is_header(*iter, 4);
}

static ds_bool
is_subparagraph (dyniter *iter,
              element *element)
{
  assert(iter);
  return is_header(*iter, 5);
}

static ds_bool
is_subsubparagraph (dyniter *iter,
              element *element)
{
  assert(iter);
  return is_header(*iter, 6);
}

static ds_bool
is_italic(dyniter *it,
          element *cont)
{
  assert(it);
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
is_underline(dyniter *it,
             element *cont)
{
  assert(it);
  if (dyniter_at(*it) == '_')
  {
    dyniter prev = *it;
    dyniter next = *it;
    dyniter_next(&next);
    if (dyniter_at(next) != '\n' &&
        !is_spacer(dyniter_at(next))) {
      prev = next;
      while (dyniter_next(&next)) {
        if (dyniter_at(next) == '_' &&
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
is_midleline(dyniter *it,
             element *cont)
{
  assert(it);
  if (dyniter_at(*it) == '~')
  {
    dyniter prev = *it;
    dyniter next = *it;
    dyniter_next(&next);
    if (dyniter_at(next) != '\n' &&
        !is_spacer(dyniter_at(next))) {
      prev = next;
      while (dyniter_next(&next)) {
        if (dyniter_at(next) == '~' &&
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
  assert(it);
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


static dynrange
head_range (dyniter s, size_t d)
{
  dyniter i = s;

  dyniter_end_line(&i);
  dyniter p = i;
  while (dyniter_next(&i)) {
    if (dyniter_at(i) == '#') {
      size_t c = 1;
      while (dyniter_next(&i)) {
        if (dyniter_at(i) == '#') {
           c++;
           if (c > d) {
             break;
           }
        } else if (is_spacer(dyniter_at(i))) {
          return (dynrange){s, p};
        } else break;
      }
    }
    dyniter_end_line(&i);
    p = i;
  }
  return (dynrange){s, i};
}

static element*
gen_section (dyniter *iter)
{
  assert(iter);
  dyniter start = *iter;
  element * title = gen_h_title(iter);
  dyniter_next(iter);
  dyniter inner = *iter;
  dynrange r = head_range(start, 1);

  element * e= gen_element(r, (dynrange) {inner, r.end},
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
  dynrange r = head_range(start, 2);

  element * e = gen_element(r, (dynrange) {inner, r.end},
                            "Subsection", BLOCK, SUBSECTION);
  element_append(e, title);
  return e;
}

static element*
gen_subsubsection (dyniter *iter)
{
  assert(iter);
  dyniter start = *iter;
  element * title = gen_h_title(iter);
  dyniter_next(iter);
  dyniter inner = *iter;
  dynrange r = head_range(start, 3);

  element * e = gen_element(r, (dynrange) {inner, r.end},
                            "Subsubsection", BLOCK, SUBSUBSECTION);
  element_append(e, title);
  return e;
}

static element*
gen_paragraph (dyniter *iter)
{
  assert(iter);
  dyniter start = *iter;
  element * title = gen_h_title(iter);
  dyniter_next(iter);
  dyniter inner = *iter;
  dynrange r = head_range(start, 4);

  element * e = gen_element(r, (dynrange) {inner, r.end},
                            "Paragraph", BLOCK, PARAGRAPH);
  element_append(e, title);
  return e;
}

static element*
gen_subparagraph (dyniter *iter)
{
  assert(iter);
  dyniter start = *iter;
  element * title = gen_h_title(iter);
  dyniter_next(iter);
  dyniter inner = *iter;
  dynrange r = head_range(start, 5);

  element * e = gen_element(r, (dynrange) {inner, r.end},
                            "Subparagraph", BLOCK, SUBPARAGRAPH);
  element_append(e, title);
  return e;
}

static element*
gen_subsubparagraph (dyniter *iter)
{
  assert(iter);
  dyniter start = *iter;
  element * title = gen_h_title(iter);
  dyniter_next(iter);
  dyniter inner = *iter;
  dynrange r = head_range(start, 6);

  element * e = gen_element(r, (dynrange) {inner, r.end},
                            "Subsubparagraph", BLOCK, SUBSUBPARAGRAPH);
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

static element*
gen_underline (dyniter *iter) {
  assert(iter);
  dyniter start = *iter;
  dyniter_next(iter);
  dyniter end = start;
  dyniter prev = end;
  while(dyniter_next(&end)) {
    if (dyniter_at(end) == '_' && !is_spacer(dyniter_at(prev)) ) {
      dyniter next =end;
      dyniter_next(&next);
      char n = dyniter_at(next);
      if (n == '\n' || is_spacer(n)) {
        dyniter s = start;
        dyniter_next (&s);
        dyniter e = end;
        dyniter_prev (&e);
        return gen_element((dynrange) {start, end},
                           (dynrange) {s, e},
                           "Underline", INLINE, UNDERLINE);
      }
    }
    prev = end;
  }
  return NULL;
}

static element*
gen_middleline (dyniter *iter) {
  assert(iter);
  dyniter start = *iter;
  dyniter_next(iter);
  dyniter end = start;
  dyniter prev = end;
  while(dyniter_next(&end)) {
    if (dyniter_at(end) == '~' && !is_spacer(dyniter_at(prev)) ) {
      dyniter next =end;
      dyniter_next(&next);
      char n = dyniter_at(next);
      if (n == '\n' || is_spacer(n)) {
        dyniter s = start;
        dyniter_next (&s);
        dyniter e = end;
        dyniter_prev (&e);
        return gen_element((dynrange) {start, end},
                           (dynrange) {s, e},
                           "Midleline", INLINE, MIDDLELINE);
      }
    }
    prev = end;
  }
  return NULL;
}


static ds_bool
is_quote_block(dyniter *iter,
               element *parent)
{
  if (iter->column == 0) {
    dyniter i = *iter;

    size_t j = 0;
    do {
      char c =  dyniter_at(i);
      if (c == '\t')
        j += 4;
      else if (c == ' ')
        j ++;
      else if (j >= 4 && c != '\n')
        return TRUE;
      else
        return FALSE;
    } while (dyniter_next(&i));
  }
  return FALSE;
}

static element*
gen_quote_block(dyniter *iter)
{
  dyniter s = *iter;
  dyniter e = *iter;
  dyniter_end_line(&e);
  dyniter i = e;
  while (dyniter_next(&i)) {
    if (is_quote_block(&i, NULL)) {
      dyniter_end_line(&i);
      e = i;
    } else break;
  }
  dynrange r = {s, e};
  dynrange o = {s, s};
  element * t = text_block(s, e);
  element * qb = gen_element(r, o, "QuoteBlock", BLOCK, QUOTEBLOCK);
  element_append(qb, t);
  return qb;
}


parser
default_parser ()
{
  size_t n = 11;
  pfunc * funcs = malloc(n * sizeof *funcs);

  funcs[0] = pfunc_new (SECTION, 0, is_section, gen_section);
  funcs[1] = pfunc_new (SUBSECTION, 0, is_subsection, gen_subsection);
  funcs[2] = pfunc_new (SUBSUBSECTION, 0, is_subsubsection, gen_subsubsection);
  funcs[3] = pfunc_new (PARAGRAPH, 0, is_paragraph, gen_paragraph);
  funcs[4] = pfunc_new (SUBPARAGRAPH, 0, is_subparagraph, gen_subparagraph);
  funcs[5] = pfunc_new (SUBSUBPARAGRAPH, 0, is_subsubparagraph, gen_subsubparagraph);

  funcs[6] = pfunc_new (ITALIC, 0, is_italic, gen_italic);
  funcs[7] = pfunc_new (BOLD, 0, is_bold, gen_bold);
  funcs[8] = pfunc_new (UNDERLINE, 0, is_underline, gen_underline);
  funcs[9] = pfunc_new (UNDERLINE, 0, is_midleline , gen_middleline);

  funcs[10] = pfunc_new (QUOTEBLOCK, 0, is_quote_block, gen_quote_block);
  return (parser){funcs, n};
};
