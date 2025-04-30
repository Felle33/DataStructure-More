#include "segment_tree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Initialize an empty segment tree with size leaf nodes
Seg_tree* init_seg_tree_empty(uint32_t size) {
  assert(size > 0 && "The tree should have at least a size greater than 0");
  // size should be a power of 2
  uint32_t pow2 = 1;
  while(pow2 < size) {
    pow2 *= 2;
  }

  size = pow2;
  Seg_tree* sgt = malloc(sizeof(Seg_tree));
  sgt->nodes = malloc(sizeof(int) * size);
  sgt->size = size;
  
  memset(sgt->nodes, 0, sizeof(int) * size);
  return sgt;
}

// Initialize a segment tree with elements of the array as leaf nodes
Seg_tree* init_seg_tree(int* array, uint32_t size);

bool is_leaf(uint32_t size_row_leaves_subtree) {
  return size_row_leaves_subtree <= 1;
}

bool is_in_left_subtree(uint32_t node, uint32_t size_row_leaves_subtree) {
  return node < size_row_leaves_subtree / 2;
}

static void point_update_rec(Seg_tree* sgt, uint32_t size_row_leaves_subtree, uint32_t cur_node, uint32_t fin_node, int value) {
  assert(cur_node > 0 && cur_node < sgt->size && "In point_update_rec the current position should be between 0 and the size of the segment tree");

  if(is_leaf(size_row_leaves_subtree)) {
    sgt->nodes[cur_node] = value;
    return;
  }

  if(is_in_left_subtree(fin_node, size_row_leaves_subtree)) point_update_rec(sgt, size_row_leaves_subtree / 2, 2 * cur_node, fin_node, value);
  else point_update_rec(sgt, size_row_leaves_subtree / 2, 2 * cur_node + 1, fin_node - size_row_leaves_subtree / 2, value);

  sgt->nodes[cur_node] = sgt->nodes[2 * cur_node] + sgt->nodes[2 * cur_node + 1];
}

// Update a certain leaf node starting from 0 with the value
void point_update_seg_tree(Seg_tree* sgt, uint32_t pos, int value) {
  point_update_rec(sgt, sgt->size / 2, 1, pos, value);
}

// Given the left and the right position calculate the f(left...right)
// right not included
// TODO: add the function f to combine the 2 subtrees
int range_query_seg_tree(Seg_tree* sgt, uint32_t left, uint32_t right);
