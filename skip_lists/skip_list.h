#ifndef _SKIP_LIST_
#define _SKIP_LIST_

#include <stdbool.h>

#ifdef USE_ARENA_ALLOC
#include <stdlib.h>
#include "arena.h"
#define SKIP_LIST_ARENAS_NUM 11
#endif

typedef struct skip_list {
  int val;
  struct skip_list* next;
  struct skip_list* bottom_layer;
} skip_list;

typedef struct skip_list_hdr {
  skip_list* head;
#ifdef USE_ARENA_ALLOC
  Arena arenas[SKIP_LIST_ARENAS_NUM];
#endif
} skip_list_hdr;

skip_list_hdr* init_skip_list();
void insert_skip_list(skip_list_hdr* hdr, int el);
bool search_skip_list(skip_list_hdr* hdr, int el);

#endif // _SKIP_LIST_
