#include "segment_tree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Initialize an empty segment tree with size leaf nodes
Seg_tree* init_seg_tree_empty(uint32_t size, int neutral_element, int (*f)(int, int)) {
  assert(size > 0 && "The tree should have at least a size greater than 0");
  // size should be a power of 2
  uint32_t pow2 = 1;
  while(pow2 < size) {
    pow2 *= 2;
  }

  // Because if i want a segment tree of 8 cells, I need at least 16 cells
  size = 2 * pow2;
  Seg_tree* sgt = malloc(sizeof(Seg_tree));
  sgt->nodes = malloc(sizeof(int) * size);
  sgt->size = size;
  sgt->neutral_element = neutral_element;
  sgt->f = f;

  for(uint32_t i = 0; i < size; i++) {
    sgt->nodes[i] = neutral_element;
  }
  return sgt;
}

static bool is_leaf(uint32_t l_tree, uint32_t r_tree) {
  return r_tree - l_tree <= 1;
}

static bool is_in_left_subtree(uint32_t fin_node, uint32_t l_tree, uint32_t r_tree) {
  return fin_node < (l_tree + r_tree) / 2;
}

static void rec_init_seg_tree(Seg_tree* sgt, int* array, uint32_t arr_size, uint32_t cur_node, uint32_t l_tree, uint32_t r_tree) {
  if(is_leaf(l_tree, r_tree)) {
    if(l_tree >= arr_size) return;
    sgt->nodes[cur_node] = array[l_tree];
    return;
  }

  uint32_t left_node = 2 * cur_node;
  uint32_t right_node = 2 * cur_node + 1;
  rec_init_seg_tree(sgt, array, arr_size, left_node, l_tree, (l_tree + r_tree) / 2);
  rec_init_seg_tree(sgt, array, arr_size, right_node, (l_tree + r_tree) / 2, r_tree);
  
  sgt->nodes[cur_node] = sgt->f(sgt->nodes[left_node], sgt->nodes[right_node]);
}

// Initialize a segment tree with elements of the array as leaf nodes
Seg_tree* init_seg_tree(int* array, uint32_t arr_size, uint32_t neutral_element, int (*f)(int, int)) {
  Seg_tree* sgt = init_seg_tree_empty(arr_size, neutral_element, f);
  rec_init_seg_tree(sgt, array, arr_size, 1, 0, sgt->size / 2);
  return sgt;
}

static void point_update_rec(Seg_tree* sgt, uint32_t cur_node, uint32_t l_tree, uint32_t r_tree, uint32_t fin_node, int value) {
  assert(cur_node > 0 && cur_node < sgt->size && "In point_update_rec the current position should be between 0 and the size of the segment tree");

  if(is_leaf(l_tree, r_tree)) {
    sgt->nodes[cur_node] = value;
    return;
  }

  uint32_t left_node = 2 * cur_node;
  uint32_t right_node = 2 * cur_node + 1;
  if(is_in_left_subtree(fin_node, l_tree, r_tree)) point_update_rec(sgt, left_node, l_tree, (l_tree + r_tree) / 2, fin_node, value);
  else point_update_rec(sgt, right_node, (l_tree + r_tree) / 2, r_tree, fin_node, value);

  sgt->nodes[cur_node] = sgt->f(sgt->nodes[left_node], sgt->nodes[right_node]);
}

// Update a certain leaf node starting from 0 with the value
void point_update_seg_tree(Seg_tree* sgt, uint32_t pos, int value) {
  point_update_rec(sgt, 1, 0, sgt->size / 2, pos, value);
}

static int range_query_rec(Seg_tree* sgt, uint32_t cur_node, uint32_t l_tree, uint32_t r_tree, uint32_t l_query, uint32_t r_query) {
  assert(cur_node > 0 && cur_node < sgt->size && "In range_query_rec the current position should be between 0 and the size of the segment tree");
  
  if(l_query <= l_tree && r_tree <= r_query) {
    return sgt->nodes[cur_node];
  }

  if(r_query <= l_tree || l_query >= r_tree) {
    return sgt->neutral_element;
  }

  return sgt->f(range_query_rec(sgt, 2 * cur_node, l_tree, (l_tree + r_tree) / 2, l_query, r_query),
		range_query_rec(sgt, 2 * cur_node + 1, (l_tree + r_tree) / 2, r_tree, l_query, r_query));
}

// Given the left and the right position calculate the f(left...right)
// right not included
int range_query_seg_tree(Seg_tree* sgt, uint32_t left, uint32_t right) {
  return range_query_rec(sgt, 1, 0, sgt->size / 2, left, right);
}

void free_seg_tree(Seg_tree* sgt) {
  free(sgt->nodes);
  free(sgt);
}

void print_debug_seg_tree(Seg_tree* sgt) {
  printf("[DEBUG] Segment tree size %d\n", sgt->size);
  for(uint32_t i = 1; i < sgt->size; i++) {
    printf("[DEBUG] node %d value %d\n", i, sgt->nodes[i]);
  }
}
