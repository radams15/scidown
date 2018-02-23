#include "ast.h"

#include <stdlib.h>
#include <stdio.h>

element *
element_new    (edef     definition,
                dynrange range)
{
  element * el = malloc(sizeof *el);
  el->child = NULL;
  el->next = NULL;
  el->definition = definition;
  el->range = range;
  return el;
}

element *
element_add    (element *root,
                element *elmnt)
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

element *
element_append (element *root,
                element *elmnt)
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

void
ast_print      (element    *el,
                const char *del)
{
  dynstr * s = dynstr_substr_r(el->range);
  if (s){
    printf("%s [%s]  %s \n", del, el->definition.name, dynstr_print(s));
    dynstr_free(s);
  }else
    printf("%s [%s]\n", del, el->definition.name);

  if (el->child)
  {
    char neu [128];
    sprintf(neu, "%s\t", del);
    ast_print(el->child, neu);
  }
  if (el->next)
  {
    ast_print(el->next, del);
  }
}
