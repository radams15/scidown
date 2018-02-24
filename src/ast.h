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
  SECTION,        /*x*/
  SUBSECTION,     /*x*/
  SUBSUBSECTION,  /*x*/
  PARAGRAPH,      /*x*/
  SUBPARAGRAPH,   /*x*/
  SUBSUBPARAGRAPH,/*x*/

  HTITLE,         /*x*/
  LANG,           /*x*/
  ITITLE,         /*x*/
  ALT,            /*x*/
  URL,            /*x*/
  REFID,          /*x*/

  /** block elements **/
  TEXT,           /*x*/
  CODE,           /*x*/
  QUOTEBLOCK,     /*x*/
  MATHBLOCK,      /*x*/
  TABLE,
  CITEDEF,        /*x*/


  /** inline elements **/
  BOLD,           /*x*/
  ITALIC,         /*x*/
  MONO,           /*x*/
  UNDERLINE,      /*x*/
  MIDDLELINE,     /*x*/
  QUOTE,          /*x*/
  LINK,           /*x*/
  IMAGE,          /*x*/
  CITE,           /*x*/
  SUPERSCRIPT,    /*x*/
  MATH            /*x*/
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
  COLOR
} typedef SCIDOWN;


struct element {
  edef     definition;
  dynrange range;
  dynrange inner;
  void     *child;
  void     *next;
} typedef element;

element * element_new    (edef        definition,
                          dynrange    range,
                          dynrange    inner);

void      element_free   (element    *el);

element * element_add    (element    *root,
                          element    *elmnt);

element * element_append (element    *root,
                          element    *elmnt);
void      ast_print      (element    *el,
                          const char *del);
#endif
