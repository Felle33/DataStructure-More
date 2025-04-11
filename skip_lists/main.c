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
  skip_list* sl = init_skip_list();

  srand(time(NULL));
  
  clock_t start, end;
  double time_passed;
  start = clock();
  int elements[DIM];
  for(size_t i = 0; i < DIM; i++) {
    elements[i] = rand();
    insert_skip_list(sl, elements[i]);
  }
  end = clock();
  time_passed = (double)(end - start) / CLOCKS_PER_SEC;

  printf("Insertion of %ld elements in %.3f seconds\n", DIM, time_passed);
#ifdef DEBUG
  print_list(elements, DIM);
#endif

  start = clock();
  qsort(elements, DIM, sizeof(int), cmp);
  end = clock();
  time_passed = (double)(end - start) / CLOCKS_PER_SEC;

  printf("Sorting the array in %.3f seconds\n", time_passed);
  
#ifdef DEBUG
  print_list(elements, DIM)
#endif

  for(size_t test = 1; test <= NUM_TESTS; test++) {
    bool search_mem_elements = rand() % 2;
    printf("\n[Test number %ld]: %s\n", test, search_mem_elements ? "Searching memorized elements" : "Not searching for memorized elements");
    
    size_t elements_searched = 0;
    start = clock();

    if(search_mem_elements) {
      elements_searched = DIM;
      for(size_t i = 0; i < DIM; i++) {
#ifdef DEBUG
	printf("Searching the number in the array %d\n", elements[i]);
#endif
	assert(search_skip_list(sl, elements[i]));
      }
    } else {
      for(size_t i = 0; i < DIM; i++) {
	int el = rand();
      
#ifdef DEBUG
	printf("Searching the number %d\n", elements[i]);
#endif
	if(!in_list(elements, DIM, el)) {
	  elements_searched += 1;
	  assert(!search_skip_list(sl, el));
	}
      }
    }

    end = clock();
    time_passed = (double)(end - start) / CLOCKS_PER_SEC;
  
    printf("Search of %ld/%ld %s elements in %.3f seconds\n", elements_searched, DIM, search_mem_elements ? "present" : "not present", time_passed); 
  }

  printf("\n\nAll test have passed!\n");
  return 0;
}
