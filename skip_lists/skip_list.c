#include "skip_list.h"
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <assert.h>
#include <limits.h>

static const size_t MAX_HEIGHT = 20;

skip_list* init_skip_list() {
  srand(time(NULL));
  skip_list* sl;
  skip_list* bottom_layer = NULL;
  for(size_t i = 0; i < MAX_HEIGHT; i++) {
    sl = malloc(sizeof(skip_list));
    sl->val = INT_MIN;
    sl->next = NULL;
    sl->bottom_layer = bottom_layer;
    bottom_layer = sl;
  }

  return sl;
}

// It should always return a non NULL pointer and the value of the node
// return shoud be less or equal to el
static skip_list* search_skip_list_helper(skip_list* sl, int el) {
  skip_list* prev = sl;
 
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
  return search_skip_list_helper(prev->bottom_layer, el);
}

bool search_skip_list(skip_list* sl, int el) {
  skip_list* node_searched = search_skip_list_helper(sl, el);
  return node_searched->val != el ? false : true;
}

// We can use a trick to understand if the bottom layer exists
// We can return true if the bottom layer has been allocated otherwise false
static void insert_skip_list_helper(skip_list* sl, int el, bool* has_bottom_layer, skip_list** bottom_node) {
  skip_list* prev = sl;
  
  if(sl->bottom_layer == NULL) {
    // we are in the last layer
    while(sl != NULL && sl->val < el) {
      prev = sl;
      sl = sl->next;
    }

    // now sl is NULL or sl->val > el
    // alloc a new node and link
    skip_list* new_bottom_node = malloc(sizeof(skip_list));
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
    skip_list* intermediate_node = malloc(sizeof(skip_list));
    intermediate_node->val = el;
    intermediate_node->bottom_layer = *bottom_node;
    intermediate_node->next = prev->next;

    prev->next = intermediate_node;

    *bottom_node = intermediate_node;
  }
  
  *has_bottom_layer = alloc_new_layer;
}

// I think we should traverse the skip_list recursively in order to
// append at each level eventually
void insert_skip_list(skip_list* sl, int el) { 
  // If there is already the element, return
  if(search_skip_list(sl, el)) return;

  bool has_bottom_layer = false;
  skip_list* bottom_node = NULL;
  insert_skip_list_helper(sl, el, &has_bottom_layer, &bottom_node);
}
