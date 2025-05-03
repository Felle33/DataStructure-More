#ifndef PERSISTENT_SEGMENT_TREE_H
#define PERSISTENT_SEGMENT_TREE_H

#include <stdint.h>

typedef struct Seg_tree_node {
  struct Seg_tree_node* left;
  struct Seg_tree_node* right;
  int64_t value;
} Seg_tree_node;

typedef struct Seg_tree {
  uint32_t size;
  int64_t neutral_element;
  int64_t (*f)(int64_t, int64_t);
  Seg_tree_node* root;
} Seg_tree;

// Initialize an empty segment tree with size leaf nodes
Seg_tree* init_seg_tree_empty(uint32_t size, int64_t neutral_element, int64_t (*f)(int64_t, int64_t));

// Initialize a segment tree with elements of the array as leaf nodes
Seg_tree* init_seg_tree(int64_t* array, uint32_t size, uint32_t neutral_element, int64_t (*f)(int64_t, int64_t));

// Update a certain leaf node starting from 0 with the value
Seg_tree* point_update_seg_tree(Seg_tree* sgt, uint32_t pos, int64_t value);

// Given the left and the right position calculate the f(left...right)
// right not included
int64_t range_query_seg_tree(Seg_tree* sgt, uint32_t left, uint32_t right);

Seg_tree* copy_seg_tree(Seg_tree* sgt);

void print_debug_seg_tree(Seg_tree* sgt);

#endif // PERSISTENT_SEGMENT_TREE_H
