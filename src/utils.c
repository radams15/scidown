#include "utils.h"
#include <stdlib.h>

Strings* add_string(Strings* head, char* str)
{
  if (head == 0) {
    head = malloc(sizeof(Strings));
    head->size = 1;
    head->str = str;
    head->next = 0;
    return head;
  }
  head->size ++;
  if (head->next) {
    add_string(head->next, str);
  } else {
    Strings * next = malloc(sizeof(Strings));
    next->size = 1;
    next->str = str;
    next->next = 0;
    head->next = next;
  }
  return head;
}
