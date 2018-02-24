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
      dyniter pp = next;
      dyniter_next(&next);
      prev = next;
      while (dyniter_next(&next)) {
        if (dyniter_at(next) == '*' &&
            dyniter_at(prev) == '*' &&
            !is_spacer(dyniter_at(pp))) {
          return TRUE;
        }
        if (dyniter_at(next) == '\n')
          return FALSE;
        pp = prev;
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
  char c = dyniter_at(start);
  dyniter end = start;
  while(dyniter_next(&end)) {
    if (dyniter_at(end) == c) {
      dyniter next =end;
      dyniter_next(&next);
      char n = dyniter_at(next);
      dyniter s = start;
      dyniter_next (&s);
      dyniter e = end;
      dyniter_prev (&e);
      return gen_element((dynrange) {start, end},
                         (dynrange) {s, e},
                         "Italic", INLINE, ITALIC);

    }
  }
  return NULL;
}


static element*
gen_bold (dyniter *iter) {
  assert(iter);
  dyniter start = *iter;
  char c = dyniter_at(start);
  dyniter end = start;
  dyniter_skip(&end, 2);
  dyniter pp = end;
  dyniter prev = end;
  dyniter next;
  while(dyniter_next(&end)) {
    next = end;
    if (!dyniter_next(&next) || (dyniter_at(end) == c && dyniter_at(prev) == c
                                 && dyniter_at(next) != c && !is_spacer(dyniter_at(pp)))) {
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
    pp = prev;
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



static ds_bool
is_code_block(dyniter *iter,
              element *parent)
{
  if (iter->column == 0) {
    dyniter i = *iter;
    char c = dyniter_at(i);
    if (c == '~' || c =='`') {
      dyniter_next(&i);
      size_t j;
      for (j = 1; j < 3; j++) {
        if (dyniter_at(i) != c)
          return FALSE;
        if (!dyniter_next(&i))
          return FALSE;
      }
      if (dyniter_at(i) != c)
        return TRUE;
    }
  }
  return FALSE;
}

static element*
gen_code_block (dyniter *iter)
{
  dyniter start = *iter;
  char c = dyniter_at(start);
  dyniter_skip(iter, 3);
  char k = dyniter_at(*iter);
  element * lang = NULL;
  if (!is_spacer(k) && k != '\n')
  {
    dyniter s = *iter;
    while(dyniter_next(iter))
    {
      k = dyniter_at(*iter);
      if (is_spacer(k) || k == '\n') {
        break;
      }
    }
    dyniter e = *iter;
    dyniter_prev(&e);
    lang = gen_sin_element(s, e, "Lang", INLINE, LANG);
  }
  dyniter_end_line(iter);
  while (dyniter_next(iter)) {
    k = dyniter_at(*iter);
    if (k == c) {
      size_t j = 1;
      ds_bool stat = FALSE;
      while (dyniter_next(iter)) {
        j ++;
        k = dyniter_at(*iter);
        if (k == c && j == 4)
          break;
        if (k != c && j < 4)
          break;
        if ((is_spacer(k) || k == '\n') && j == 4) {
          stat = TRUE;
          break;
        }
      }
      if (stat)
        break;
    }
    dyniter_end_line(iter);
  }
  dyniter end = *iter;

  dyniter s = start;
  dyniter e = end;
  dyniter_end_line(&s);
  dyniter_next(&s);
  dyniter_goto(&e, e.i -3);

  element * t = text_block(s, e);
  element * bq = gen_element((dynrange) {start, end}, (dynrange) {start, start}, "CodeBlock", BLOCK, CODE);
  if (lang)
    element_append(bq, lang);
  element_append(bq, t);
  return bq;
}

static ds_bool
is_math_block(dyniter *it,
              element *parent)
{
  if (dyniter_at(*it) == '$') {
    dyniter i = *it;
    dyniter_next(&i);
    if (dyniter_at(i) == '$') {
      return TRUE;
    }
  }
  return FALSE;
}

static element *
gen_math_block (dyniter *it)
{
  dyniter s = *it;
  dyniter i = s;
  dyniter_skip(&i, 2);
  size_t c = 2;
  while (dyniter_next(&i)) {
    char k = dyniter_at(i);
    if (k == '\n') {
      if (!c)
        return NULL;;
      c = 0;
    } else if (k =='$') {
      dyniter n = i;
      dyniter_next(&n);
      if (dyniter_at(n) == '$') {
        dyniter ts = s;
        dyniter_skip(&ts, 2);
        dyniter te = i;
        dyniter_prev(&te);
        element * txt = text_block(ts, te);
        element * math = gen_element((dynrange) {s, n}, (dynrange) {s, s}, "MathBlock", BLOCK, MATHBLOCK);
        element_append(math, txt);
        return math;
      }
    } else if (!is_spacer(k)) {
      c++;
    }
  }
  return NULL;
}

static ds_bool
is_mono (dyniter *it,
         element *el)
{
  if (dyniter_at(*it) == '`')
  {
    dyniter i = *it;
    dyniter_next(&i);
    if (dyniter_at(i) == '`')
      return FALSE;
    while (dyniter_next(&i)) {
      char k = dyniter_at(i);
      if (k == '`')
        return TRUE;
      if (k == '\n')
        return FALSE;
    }
  }
  return FALSE;
}

static element*
gen_mono (dyniter *it)
{
  dyniter s = *it;
  while (dyniter_next(it)) {
    char k = dyniter_at(*it);
    if (k == '`')
      break;
  }
  dyniter e = *it;
  dyniter ts = s;
  dyniter te = e;
  dyniter_next(&ts);
  dyniter_prev(&te);
  return gen_element((dynrange) {s, e}, (dynrange) {ts, te}, "Mono", INLINE, MONO);
}

static ds_bool
is_image (dyniter *it,
          element *parent)
{
  if (dyniter_at(*it) == '!') {
    dyniter i = *it;
    dyniter_next(&i);
    if (dyniter_at(i) == '[') {
      while(dyniter_next(&i)) {
        char c = dyniter_at(i);
        if (c == '\n' || c == '[')
          return FALSE;
        if (c == ']')
          break;
      }
      dyniter_next(&i);
      if (dyniter_at(i) == '(') {
        while (dyniter_next(&i)) {
          char c = dyniter_at(i);
          if (c == '\n' || c == '(')
            return FALSE;
          if (c == ')')
            return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static element*
gen_image (dyniter *it)
{
  dyniter s = *it;
  dyniter i = s;
  dyniter_skip(&i,2);
  dynrange alt_r = {i, i};
  element * alt = NULL;
  element * url = NULL;
  element * title = NULL;
  while(dyniter_next(&i)) {
    char c = dyniter_at(i);
    if (c == ']') {
      if (i.i > (alt_r.start.i + 1)) {
        alt_r.end = i;
        dyniter_prev(&alt_r.end);
        alt = gen_sin_element(alt_r.start, alt_r.end, "Alt", INLINE, ALT);
      }
      break;
    }
  }
  dyniter_skip(&i, 2);
  dynrange url_r = {i, i};
  dynrange tit_r = {i, i};
  ds_bool has_title = FALSE;
  while (dyniter_next(&i)) {
    char c = dyniter_at(i);
    if (c == ' ') {
      if (i.i > (url_r.start.i + 1)) {
        url_r.end = i;
        dyniter_prev(&url_r.end);
        url = gen_sin_element(url_r.start, url_r.end, "Url", INLINE, URL);
        has_title = TRUE;
        tit_r.start = i;
        dyniter_next(&tit_r.start);
      }
    }
    if (c == ')') {
      if (!has_title) {
        if (i.i > (url_r.start.i + 1)) {
          url_r.end = i;
          dyniter_prev(&url_r.end);
          url = gen_sin_element(url_r.start, url_r.end, "Url", INLINE, URL);
        }
      } else if (i.i > (tit_r.start.i + 1)) {
        tit_r.end = i;
        dyniter_prev(&tit_r.end);
        title = gen_sin_element(tit_r.start, tit_r.end, "ImgTitle", INLINE, ITITLE);
      }
      break;
    }
  }
  element * img = gen_element((dynrange) {s, i}, (dynrange) {s, s}, "Image", INLINE, IMAGE);
  if (alt)
    element_append(img, alt);
  if (url)
    element_append(img, url);
  if (title)
    element_append(img, title);
  return img;
}

static ds_bool
is_link (dyniter *it,
         element *parent)
{
  if (dyniter_at(*it) == '[') {
    dyniter i = *it;
    while(dyniter_next(&i)) {
      char c = dyniter_at(i);
      if (c == '\n' || c == '[')
        return FALSE;
      if (c == ']')
        break;
    }
    dyniter_next(&i);
    if (dyniter_at(i) == '(') {
      while (dyniter_next(&i)) {
        char c = dyniter_at(i);
        if (c == '\n' || c == '(')
          return FALSE;
        if (c == ')')
          return TRUE;
        }
    }
  }
  return FALSE;
}


static element*
gen_link (dyniter *it)
{
  dyniter s = *it;
  dyniter i = s;
  dyniter_skip(&i,1);
  dynrange alt_r = {i, i};
  element * url = NULL;
  while(dyniter_next(&i)) {
    char c = dyniter_at(i);
    if (c == ']') {
      if (i.i > (alt_r.start.i + 1)) {
        alt_r.end = i;
        dyniter_prev(&alt_r.end);
      }
      break;
    }
  }
  dyniter_skip(&i, 2);
  dynrange url_r = {i, i};

  while (dyniter_next(&i)) {
    char c = dyniter_at(i);
    if (c == ')') {
      if (i.i > (url_r.start.i + 1)) {
        url_r.end = i;
        dyniter_prev(&url_r.end);
        url = gen_sin_element(url_r.start, url_r.end, "Url", INLINE, URL);
      }
      break;
    }
  }
  element * link = gen_element((dynrange) {s, i}, alt_r, "Link", INLINE, LINK);
  if (url) {
    element_append(link, url);
    if (alt_r.end.i <= alt_r.start.i) {
      element * txt = text_block(url_r.start, url_r.end);
      element_append(link, txt);
    }
  }
  return link;
}


parser
default_parser ()
{
  size_t n = 16;
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
  funcs[11] = pfunc_new (CODE, 0, is_code_block, gen_code_block);
  funcs[12] = pfunc_new (MATHBLOCK, 0, is_math_block, gen_math_block);

  funcs[13] = pfunc_new (MONO, 0, is_mono, gen_mono);
  funcs[14] = pfunc_new (IMAGE, 0, is_image, gen_image);
  funcs[15] = pfunc_new (LINK, 0, is_link, gen_link);

  return (parser){funcs, n};
};
