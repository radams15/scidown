#ifndef __AST_H__
#define __AST_H__

#include "DynString/dynstring.h"

typedef int eid;
typedef int priority;

enum ETYPE {
  INLINE,
  BLOCK,
  FLOAT
} typedef ETYPE;

struct edef {
  const char *name;
  ETYPE       type;
  eid         id;
} typedef edef;

enum BASIC {
  ROOT,

  /** sections elements **/
  SECTION,
  SUBSECTION,
  SUBSUBSECTION,
  PARAGRAPHER,
  SUBPARAGRPAHER,

  /** block elements **/
  TEXT,
  CODE,
  QOUTEBLOCK,
  MATHBLOCK,
  TABLE,
  CITEDEF,


  /** inline elements **/
  BOLD,
  ITALIC,
  MONO,
  UNDERLINE,
  MIDDLELINE,
  COLOR,
  QUOTE,
  IMAGE,
  LINK,
  CITE,
  SUPERSCRIPT,
  UNDERSCRIPT,
  MATH
} typedef BASIC;

enum SCIDOWN {
  TITLE,
  AUTHOR,
  KEYWORDS,
  AFFILIATION,
  ABSTRACT,
  FLOAT_TABLE,
  FLOAT_FIGURE,
  FLOAT_LISTING,
  EQUATION,
  REFERENCE,
  TOC,
  CAPTION,
} typedef SCIDOWN;


struct element {
  edef     definition;
  dynrange range;
  void     *child;
  void     *next;
} typedef element;

element * element_new    (edef        definition,
                          dynrange    range);

element * element_add    (element    *root,
                          element    *elmnt);

element * element_append (element    *root,
                          element    *elmnt);
void      ast_print      (element    *el,
                          const char *del);
#endif
