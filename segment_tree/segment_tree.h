#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <stdint.h>

typedef uint32_t (*f)(uint32_t, uint32_t);

typedef struct Seg_tree {
  uint32_t size;
  uint32_t neutral_element;
  uint32_t (*f)(uint32_t, uint32_t);
  int* nodes;
} Seg_tree;

// Initialize an empty segment tree with size leaf nodes
Seg_tree* init_seg_tree_empty(uint32_t size, uint32_t neutral_element, uint32_t (*f)(uint32_t, uint32_t));

// Initialize a segment tree with elements of the array as leaf nodes
Seg_tree* init_seg_tree(int* array, uint32_t size);

// Update a certain leaf node starting from 0 with the value
void point_update_seg_tree(Seg_tree* sgt, uint32_t pos, int value);

// Given the left and the right position calculate the f(left...right)
// right not included
// TODO: add the function f to combine the 2 subtrees
int range_query_seg_tree(Seg_tree* sgt, uint32_t left, uint32_t right);

void free_segment_tree(Seg_tree* sgt);

void print_debug_seg_tree(Seg_tree* sgt);

#endif // SEGMENT_TREE_H
