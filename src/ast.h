#ifndef __AST_H__
#define __AST_H__

#include "DynString/dynstring.h"

typedef int eid;
typedef int Priority;

enum EType {
  INLINE,
  BLOCK,
  FLOAT
} typedef EType;

struct EDef {
  const char *name;
  EType       type;
  eid         id;
  Priority    prioirty;
} typedef EDef;

enum BASIC {
  /** block elements **/
  SECTION,
  SUBSECTION,
  SUBSUBSECTION,
  PARAGRAPHER,
  SUBPARAGRPAHER,
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


struct Element {
  EDef    *definition;
  dyniter *source_iter;
  Element *child;
  Element *next;
} typedef Element;

Element * element_new    (EDef    *definition,
                          dyniter *iter);

Element * element_add    (Element *root,
                          Element *elmnt);

Element * element_append (Element *root,
                          Element *elmnt);
#endif
