#ifndef _SKIP_LIST_
#define _SKIP_LIST_

#include <stdbool.h>

typedef struct skip_list {
  int val;
  struct skip_list* next;
  struct skip_list* bottom_layer;
} skip_list;

skip_list* init_skip_list();
void insert_skip_list(skip_list* sl, int el);
bool search_skip_list(skip_list* sl, int el);

#endif // _SKIP_LIST_
