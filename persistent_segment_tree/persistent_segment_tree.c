// Tested with the problem Range Queries and Copies of CSES https://cses.fi/problemset/task/1737/

#include "persistent_segment_tree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static Seg_tree* init_seg_tree_struct(uint32_t size, int64_t neutral_element, int64_t (*f)(int64_t, int64_t), Seg_tree_node* root) {
  Seg_tree* sgt = malloc(sizeof(Seg_tree));
  sgt->size = size;
  sgt->neutral_element = neutral_element;
  sgt->f = f;
  sgt->root = root;
  return sgt;
}

static Seg_tree_node* init_seg_tree_node_struct(Seg_tree_node* left, Seg_tree_node* right, int64_t value) {
  Seg_tree_node* node = malloc(sizeof(Seg_tree_node));
  node->left = left;
  node->right = right;
  node->value = value;
  return node;
}

// Initialize an empty segment tree with size leaf nodes
Seg_tree* init_seg_tree_empty(uint32_t size, int64_t neutral_element, int64_t (*f)(int64_t, int64_t)) {
  assert(size > 0 && "The tree should have at least a size greater than 0");
  // size should be a power of 2
  uint32_t pow2 = 1;
  while(pow2 < size) {
    pow2 *= 2;
  }

  // Because if i want a segment tree of 8 cells, I need at least 16 cells
  size = 2 * pow2;
  Seg_tree_node* nodes = malloc(sizeof(Seg_tree_node) * size);
  Seg_tree* sgt = init_seg_tree_struct(size, neutral_element, f, &nodes[1]);

  for(uint32_t i = 1; i < size; i++) {
    nodes[i].value = neutral_element;
    if(i <= size / 2) {
      nodes[i].left = &nodes[2 * i];
      nodes[i].right = &nodes[2 * i + 1];
    } else {
      nodes[i].left = NULL;
      nodes[i].right = NULL;
    }
  }
  return sgt;
}

static bool is_leaf(uint32_t l_tree, uint32_t r_tree) {
  return r_tree - l_tree <= 1;
}

static bool is_in_left_subtree(uint32_t fin_node, uint32_t l_tree, uint32_t r_tree) {
  return fin_node < (l_tree + r_tree) / 2;
}

static void rec_init_seg_tree(Seg_tree* sgt, int64_t* array, uint32_t arr_size, Seg_tree_node* root, uint32_t l_tree, uint32_t r_tree) {
  if(is_leaf(l_tree, r_tree)) {
    if(l_tree >= arr_size) return;
    root->value = array[l_tree];
    return;
  }

  rec_init_seg_tree(sgt, array, arr_size, root->left, l_tree, (l_tree + r_tree) / 2);
  rec_init_seg_tree(sgt, array, arr_size, root->right, (l_tree + r_tree) / 2, r_tree);
  
  root->value = sgt->f(root->left->value, root->right->value);
}

// Initialize a segment tree with elements of the array as leaf nodes
Seg_tree* init_seg_tree(int64_t* array, uint32_t arr_size, uint32_t neutral_element, int64_t (*f)(int64_t, int64_t)) {
  Seg_tree* sgt = init_seg_tree_empty(arr_size, neutral_element, f);
  rec_init_seg_tree(sgt, array, arr_size, sgt->root, 0, sgt->size / 2);
  return sgt;
}

static Seg_tree_node* point_update_rec(Seg_tree* sgt, Seg_tree_node* root, uint32_t l_tree, uint32_t r_tree, uint32_t fin_node, int64_t value) {
  assert(root != NULL && "In point_update_rec the node should not be NULL");

  if(is_leaf(l_tree, r_tree)) {
    return init_seg_tree_node_struct(NULL, NULL, value);
  }

  if(is_in_left_subtree(fin_node, l_tree, r_tree)) {
    Seg_tree_node* new_left = point_update_rec(sgt, root->left, l_tree, (l_tree + r_tree) / 2, fin_node, value);
    return init_seg_tree_node_struct(new_left, root->right, sgt->f(new_left->value, root->right->value));
  } else {
    Seg_tree_node* new_right = point_update_rec(sgt, root->right, (l_tree + r_tree) / 2, r_tree, fin_node, value);
    return init_seg_tree_node_struct(root->left, new_right, sgt->f(root->left->value, new_right->value));
  }
}

// Update a certain leaf node starting from 0 with the value
Seg_tree* point_update_seg_tree(Seg_tree* sgt, uint32_t pos, int64_t value) {
  return init_seg_tree_struct(sgt->size, sgt->neutral_element, sgt->f, point_update_rec(sgt, sgt->root, 0, sgt->size / 2, pos, value));
}

static int64_t range_query_rec(Seg_tree* sgt, Seg_tree_node* root, uint32_t l_tree, uint32_t r_tree, uint32_t l_query, uint32_t r_query) {
  assert(root != NULL && "In point_update_rec the node should not be NULL");
  
  if(l_query <= l_tree && r_tree <= r_query) {
    return root->value;
  }

  if(r_query <= l_tree || l_query >= r_tree) {
    return sgt->neutral_element;
  }

  return sgt->f(range_query_rec(sgt, root->left, l_tree, (l_tree + r_tree) / 2, l_query, r_query),
		range_query_rec(sgt, root->right, (l_tree + r_tree) / 2, r_tree, l_query, r_query));
}

// Given the left and the right position calculate the f(left...right)
// right not included
int64_t range_query_seg_tree(Seg_tree* sgt, uint32_t left, uint32_t right) {
  return range_query_rec(sgt, sgt->root, 0, sgt->size / 2, left, right);
}

Seg_tree* copy_seg_tree(Seg_tree* sgt) {
  return sgt;
}

static void rec_print_debug_seg_tree(Seg_tree_node* root, uint32_t cur_node) {
  if(root == NULL) return;
  printf("[DEBUG] node %d value %ld\n", cur_node, root->value);
  rec_print_debug_seg_tree(root->left, 2 * cur_node);
  rec_print_debug_seg_tree(root->right, 2 * cur_node + 1);
}

void print_debug_seg_tree(Seg_tree* sgt) {
  printf("[DEBUG] Segment tree size %d\n", sgt->size);
  rec_print_debug_seg_tree(sgt->root, 1);
}

int main(void) {
  return 0;
}
