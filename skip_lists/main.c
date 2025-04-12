#include "skip_list.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

const size_t DIM = 1000000;
const size_t NUM_TESTS = 4;

bool in_list(int* elements, size_t n, int el) {
  int l = -1, r = n;
  while(l + 1 < r) {
    int mid = l + (r - l) / 2;
    if(elements[mid] <= el) {
      l = mid;
    } else {
      r = mid;
    }
  }
  return elements[l] == el;
}

void print_list(int* elements, size_t n) {
  for(size_t i = 0; i < n; i++) {
    printf("[%ld] = %d\n", i, elements[i]);
  }
}

int cmp(const void* a, const void* b) {
  int* a_int = (int*) a;
  int* b_int = (int*) b;

  return *a_int - *b_int;
}

int main(void) {
  clock_t start, end;
  double time_passed;
  int to_search_arr[DIM], not_to_search_arr[DIM];
  
  srand(time(NULL));
  skip_list_hdr* hdr = init_skip_list();

  start = clock();
  for(size_t i = 0; i < DIM; i++) {
    to_search_arr[i] = rand();
  }
  qsort(to_search_arr, DIM, sizeof(int), cmp);

  for(size_t i = 0; i < DIM; i++) {
    int el = rand();
    while(in_list(to_search_arr, DIM, el)) {
      el = rand();
    }
    not_to_search_arr[i] = el;
  }
  end = clock();  
  time_passed = (double)(end - start) / CLOCKS_PER_SEC;

  printf("Initialized the arrays with DIM=%ld in %.3f seconds\n\n", DIM, time_passed);

  start = clock();
  for(size_t i = 0; i < DIM; i++) {
    insert_skip_list(hdr, to_search_arr[i]);
  }
  end = clock();
  time_passed = (double)(end - start) / CLOCKS_PER_SEC;

  printf("Insertion of %ld elements in %.3f seconds\n", DIM, time_passed);
#ifdef DEBUG
  print_list(to_search_arr, DIM);
#endif
  
  for(size_t test = 1; test <= NUM_TESTS; test++) {
    bool search_mem_elements = rand() % 2;
    printf("\n[Test number %ld]: %s\n", test, search_mem_elements ? "Searching memorized elements" : "Not searching for memorized elements");
    
    start = clock();

    if(search_mem_elements) {
      for(size_t i = 0; i < DIM; i++) {
#ifdef DEBUG
	printf("Searching the number in the array %d\n", to_search_arr[i]);
#endif
	assert(search_skip_list(hdr, to_search_arr[i]));
      }
    } else {
      for(size_t i = 0; i < DIM; i++) {
#ifdef DEBUG
	printf("Searching the number (that does not exists) %d\n", elements[i]);
#endif
	assert(!search_skip_list(hdr, not_to_search_arr[i]));
      }
    }

    end = clock();
    time_passed = (double)(end - start) / CLOCKS_PER_SEC;
  
    printf("Search of %s elements in %.3f seconds\n", search_mem_elements ? "present" : "not present", time_passed); 
  }

  printf("\n\nAll tests have passed!\n");
  return 0;
}
