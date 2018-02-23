#include "ast.h"

#include <stdlib.h>

Element *
element_new    (EDef    *definition,
                dyniter *iter)
{
  Element * el = malloc(sizeof *el);
  el->child = NULL;
  el->next = NULL;
  el->definition = definition;
  el->source_iter = iter;
  return el;
}

Element *
element_add    (Element *root,
                Element *elmnt)
{
  if (root == NULL) {
    return elmnt;
  } else {
    if (root->next)
      element_add(root->next, elmnt);
    else
      root->next = elmnt;
    return root;
  }
}

Element *
element_append (Element *root,
                Element *elmnt)
{
  if (root == NULL) {
    return elmnt;
  } else {
    if (root->child)
      element_add(root->child, elmnt);
    else
      root->child = elmnt;
    return root;
  }
}
