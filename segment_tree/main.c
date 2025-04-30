#include "segment_tree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

const int SIZE = 33;

int range_query_arr(int* arr, int l, int r) {
  int res = 0;
  for(int i = l; i < r; i++) {
    res += arr[i];
  }
  return res;
}

uint32_t sum(uint32_t a, uint32_t b) {
  return a + b;
}

int main(void) {
  Seg_tree *sgt_sum = init_seg_tree_empty(SIZE, 0, sum);
  int* arr = malloc(sizeof(int) * SIZE);
  
  for(int i = 0; i < SIZE; i++) {
    int val = i + 1;
    point_update_seg_tree(sgt_sum, i, val);
    arr[i] = val;
  }

  // print_debug_seg_tree(sgt_sum);

  for(int i = 0; i < SIZE; i++) {
    for(int j = i + 1; j <= SIZE; j++) {
      int sgt_sum_res = range_query_seg_tree(sgt_sum, i, j);
      int arr_res = range_query_arr(arr, i, j);

      assert(sgt_sum_res == arr_res && "The segment tree and array computations should be equal");
    }
  }

  printf("All the assertions are true\n");
  return 0;
}
