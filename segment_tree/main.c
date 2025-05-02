// Tested also with the problem Dynamic Range Sum Queries of CSES https://cses.fi/problemset/task/1648/

#include "segment_tree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

const int SIZE = 101;

int range_query_arr(int* arr, int l, int r, int neutral_element, int (*f)(int, int)) {
  int res = neutral_element;
  for(int i = l; i < r; i++) {
    res = f(res, arr[i]);
  }
  return res;
}

int sum(int a, int b) {
  return a + b;
}

int max(int a, int b) {
  return a >= b ? a : b;
}

int main(void) {
  // Seg_tree *sgt_sum = init_seg_tree_empty(SIZE, 0, sum);
  int* arr_sum = malloc(sizeof(int) * SIZE);
  for(int i = 0; i < SIZE; i++) {
    int val = i + 1;
    arr_sum[i] = val;
  }
  
  /* for(int i = 0; i < SIZE; i++) { */
  /*   int val = i + 1; */
  /*   point_update_seg_tree(sgt_sum, i, val); */
  /*   arr_sum[i] = val; */
  /* } */

  Seg_tree *sgt_sum = init_seg_tree(arr_sum, SIZE, 0, sum);

  for(int i = 0; i < SIZE; i++) {
    assert(i + 1 == range_query_seg_tree(sgt_sum, i, i + 1) && "The elements of the segment tree should be stored");
  }

  // print_debug_seg_tree(sgt_sum);

  for(int i = 0; i < SIZE; i++) {
    for(int j = i + 1; j <= SIZE; j++) {
      int sgt_sum_res = range_query_seg_tree(sgt_sum, i, j);
      int arr_sum_res = range_query_arr(arr_sum, i, j, 0, sum);

      assert(sgt_sum_res == arr_sum_res && "The segment tree and array computations should be equal");
    }
  }

  printf("The total sum is %d\n", range_query_seg_tree(sgt_sum, 0, SIZE));
  
  free_seg_tree(sgt_sum);
  free(arr_sum);

  Seg_tree *sgt_max = init_seg_tree_empty(SIZE, 0, max);
  int* arr_max = malloc(sizeof(int) * SIZE);
  
  for(int i = 0; i < SIZE; i++) {
    int val = i + 1;
    point_update_seg_tree(sgt_max, i, val);
    arr_max[i] = val;
  }

  for(int i = 0; i < SIZE; i++) {
    assert(i + 1 == range_query_seg_tree(sgt_max, i, i + 1) && "The elements of the segment tree should be stored");
  }

  // print_debug_seg_tree(sgt_max);

  for(int i = 0; i < SIZE; i++) {
    for(int j = i + 1; j <= SIZE; j++) {
      int sgt_max_res = range_query_seg_tree(sgt_max, i, j);
      int arr_max_res = range_query_arr(arr_max, i, j, 0, max);

      assert(sgt_max_res == arr_max_res && "The segment tree and array computations should be equal");
    }
  }

  printf("The maximum is %d\n", range_query_seg_tree(sgt_max, 0, SIZE));
  
  free_seg_tree(sgt_max);
  free(arr_max);  

  printf("All the assertions are true\n");
  return 0;
}
