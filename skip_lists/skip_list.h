#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <stdbool.h>

#ifdef USE_ARENA_ALLOC
#include <stdlib.h>
#include "arena.h"
#define SKIP_LIST_ARENAS_NUM 11
#endif

#if !defined(TYPE) && !defined(TYPED_NAME)
#error "The define TYPE or TYPED_NAME is not defined"
#endif

typedef struct TYPED_NAME(skip_list) {
  TYPE val;
  struct TYPED_NAME(skip_list)* next;
  struct TYPED_NAME(skip_list)* bottom_layer;
} TYPED_NAME(skip_list);

typedef struct TYPED_NAME(skip_list_hdr) {
  TYPED_NAME(skip_list)* head;
#ifdef USE_ARENA_ALLOC
  Arena arenas[SKIP_LIST_ARENAS_NUM];
#endif
} TYPED_NAME(skip_list_hdr);

TYPED_NAME(skip_list_hdr)* TYPED_NAME(init_skip_list)(TYPE min_el);
void TYPED_NAME(insert_skip_list)(TYPED_NAME(skip_list_hdr)* hdr, TYPE el);
bool TYPED_NAME(search_skip_list)(TYPED_NAME(skip_list_hdr)* hdr, TYPE el);
void TYPED_NAME(free_skip_list)(TYPED_NAME(skip_list_hdr)* hdr);

#endif // SKIP_LIST_H

#ifdef SKIP_LIST_IMPLEMENTATION

#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <assert.h>
#include <limits.h>

static const size_t MAX_HEIGHT = 20;

TYPED_NAME(skip_list_hdr)* TYPED_NAME(init_skip_list)(TYPE min_el) {
  srand(time(NULL));
  TYPED_NAME(skip_list_hdr)* hdr = malloc(sizeof(TYPED_NAME(skip_list_hdr)));

#ifdef USE_ARENA_ALLOC
  for(size_t i = 0; i < SKIP_LIST_ARENAS_NUM; i++) {
    arena_init(&hdr->arenas[i]);
  }
#endif
  
  TYPED_NAME(skip_list)* sl;
  TYPED_NAME(skip_list)* bottom_layer = NULL;
  for(size_t i = 0; i < MAX_HEIGHT; i++) {
#ifdef USE_ARENA_ALLOC
    if(i < SKIP_LIST_ARENAS_NUM - 1) {
      sl = arena_alloc(&hdr->arenas[i], sizeof(TYPED_NAME(skip_list)));
    } else {
      sl = arena_alloc(&hdr->arenas[SKIP_LIST_ARENAS_NUM - 1], sizeof(TYPED_NAME(skip_list)));
    }
#else
    sl = malloc(sizeof(TYPED_NAME(skip_list)));
#endif

    sl->val = min_el;
    sl->next = NULL;
    sl->bottom_layer = bottom_layer;
    bottom_layer = sl;
  }

  hdr->head = sl;
  return hdr;
}

// It should always return a non NULL pointer and the value of the node
// return shoud be less or equal to el
static TYPED_NAME(skip_list)* TYPED_NAME(search_skip_list_helper)(TYPED_NAME(skip_list)* sl, TYPE el) {
  TYPED_NAME(skip_list)* prev = sl;
 
  if(sl->bottom_layer == NULL) {
    // we are in the last layer so we perform a linear scan
    while(sl != NULL && sl->val < el) {
      prev = sl;
      sl = sl->next;
    }

    if(sl != NULL && sl->val == el) return sl;
    assert(prev != NULL);
    return prev;
  }

  while(sl != NULL && sl->val < el) {
    prev = sl;
    sl = sl->next;
  }

  if(sl != NULL && sl->val == el) return sl;
  assert(prev != NULL);
  return TYPED_NAME(search_skip_list_helper)(prev->bottom_layer, el);
}

bool TYPED_NAME(search_skip_list)(TYPED_NAME(skip_list_hdr)* hdr, TYPE el) {
  TYPED_NAME(skip_list)* node_searched = TYPED_NAME(search_skip_list_helper)(hdr->head, el);
  return node_searched->val != el ? false : true;
}

#ifdef USE_ARENA_ALLOC
// We can use a trick to understand if the bottom layer exists
// We can return true if the bottom layer has been allocated otherwise false
static void TYPED_NAME(insert_skip_list_helper_arena)(Arena* arenas, TYPED_NAME(skip_list)* sl, TYPE el, bool* has_bottom_layer, TYPED_NAME(skip_list)** bottom_node, size_t depth) {
  TYPED_NAME(skip_list)* prev = sl;
  
  if(sl->bottom_layer == NULL) {
    // we are in the last layer
    while(sl != NULL && sl->val < el) {
      prev = sl;
      sl = sl->next;
    }

    // now sl is NULL or sl->val > el
    // alloc a new node and link
    TYPED_NAME(skip_list)* new_bottom_node = arena_alloc(&arenas[0], sizeof(TYPED_NAME(skip_list)));
    new_bottom_node->bottom_layer = NULL;
    new_bottom_node->next = sl;
    new_bottom_node->val = el;

    // link the old previous node
    prev->next = new_bottom_node;
    *has_bottom_layer = true;
    *bottom_node = new_bottom_node;
    return;
  }

  while(sl != NULL && sl->val < el) {
    prev = sl;
    sl = sl->next;
  }
  
  TYPED_NAME(insert_skip_list_helper_arena)(arenas, prev->bottom_layer, el, has_bottom_layer, bottom_node, depth - 1);
  bool alloc_new_layer = *has_bottom_layer && (rand() % 2);
  if(alloc_new_layer) {
    assert(*bottom_node != NULL);
    TYPED_NAME(skip_list)* intermediate_node;
    if(depth < SKIP_LIST_ARENAS_NUM - 1) {
      intermediate_node = arena_alloc(&arenas[depth], sizeof(TYPED_NAME(skip_list)));
    } else {
      intermediate_node = arena_alloc(&arenas[SKIP_LIST_ARENAS_NUM - 1], sizeof(TYPED_NAME(skip_list)));
    }
    intermediate_node->val = el;
    intermediate_node->bottom_layer = *bottom_node;
    intermediate_node->next = prev->next;

    prev->next = intermediate_node;

    *bottom_node = intermediate_node;
  }
  
  *has_bottom_layer = alloc_new_layer;
}

#else

// We can use a trick to understand if the bottom layer exists
// We can return true if the bottom layer has been allocated otherwise false
static void TYPED_NAME(insert_skip_list_helper)(TYPED_NAME(skip_list)* sl, TYPE el, bool* has_bottom_layer, TYPED_NAME(skip_list)** bottom_node) {
  TYPED_NAME(skip_list)* prev = sl;
  
  if(sl->bottom_layer == NULL) {
    // we are in the last layer
    while(sl != NULL && sl->val < el) {
      prev = sl;
      sl = sl->next;
    }

    // now sl is NULL or sl->val > el
    // alloc a new node and link
    TYPED_NAME(skip_list)* new_bottom_node = malloc(sizeof(TYPED_NAME(skip_list)));
    new_bottom_node->bottom_layer = NULL;
    new_bottom_node->next = sl;
    new_bottom_node->val = el;

    // link the old previous node
    prev->next = new_bottom_node;
    *has_bottom_layer = true;
    *bottom_node = new_bottom_node;
    return;
  }

  while(sl != NULL && sl->val < el) {
    prev = sl;
    sl = sl->next;
  }
  
  insert_skip_list_helper(prev->bottom_layer, el, has_bottom_layer, bottom_node);
  bool alloc_new_layer = *has_bottom_layer && (rand() % 2);
  if(alloc_new_layer) {
    assert(*bottom_node != NULL);
    TYPED_NAME(skip_list)* intermediate_node = malloc(sizeof(TYPED_NAME(skip_list)));
    intermediate_node->val = el;
    intermediate_node->bottom_layer = *bottom_node;
    intermediate_node->next = prev->next;

    prev->next = intermediate_node;

    *bottom_node = intermediate_node;
  }
  
  *has_bottom_layer = alloc_new_layer;
}
#endif // USE_ARENA_ALLOC


// I think we should traverse the skip_list recursively in order to
// append at each level eventually
void TYPED_NAME(insert_skip_list)(TYPED_NAME(skip_list_hdr)* hdr, TYPE el) { 
  // If there is already the element, return
  if(TYPED_NAME(search_skip_list)(hdr, el)) return;

  bool has_bottom_layer = false;
  TYPED_NAME(skip_list)* bottom_node = NULL;
  
#ifdef USE_ARENA_ALLOC
  TYPED_NAME(insert_skip_list_helper_arena)(hdr->arenas, hdr->head, el, &has_bottom_layer, &bottom_node, MAX_HEIGHT);
#else
  TYPED_NAME(insert_skip_list_helper)(hdr->head, el, &has_bottom_layer, &bottom_node);
#endif // USE_ARENA_ALLOC
}

#ifndef USE_ARENA_ALLOC
static void TYPED_NAME(free_list)(TYPED_NAME(skip_list)* sl) {
  while(sl != NULL) {
    TYPED_NAME(skip_list)* next = sl->next;
    free(sl);
    sl = next;
  }
}
#endif

void TYPED_NAME(free_skip_list)(TYPED_NAME(skip_list_hdr)* hdr) {
#ifdef USE_ARENA_ALLOC
  for(size_t i = 0; i < SKIP_LIST_ARENAS_NUM; i++) {
    arena_free(&hdr->arenas[i]);
  }
#else
  TYPED_NAME(skip_list)* sl = hdr->head;
  while(sl != NULL) {
    free_list(sl->next);

    TYPED_NAME(skip_list)* bottom = sl->bottom_layer;
    free(sl);
    sl = bottom;
  }
#endif // USE_ARENA_ALLOC
  free(hdr);
}

#endif // SKIP_LIST_IMPLEMENTATION
