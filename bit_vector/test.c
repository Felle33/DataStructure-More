#include "bit_vector.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const size_t DIM = 131;
const size_t TEST_SIZE = 10;

bool all_zeroes(bit_vector* bv) {
  for(size_t i = 0; i < DIM; i++) {
    if(access_bv(bv, i) != 0) return false;
  }
  return true;
}

bool all_ones(bit_vector* bv) {
  for(size_t i = 0; i < DIM; i++) {
    if(access_bv(bv, i) != 1) return false;
  }
  return true;
}

bool is_present(int *arr, size_t n, int x) {
  for(size_t i = 0; i < n; i++) {
    if(arr[i] == x) return true;
  }
  return false;
}

int main(void) {
  srand(time(NULL));
  bit_vector bv = init_bv(DIM);
  assert(all_zeroes(&bv) && "Not all bits are zeroes");

  set_all_bv(&bv);
  assert(all_ones(&bv) && "Not all bits are ones");

  clear_all_bv(&bv);
  assert(all_zeroes(&bv) && "Not all bits are zeroes");

  int bits_set[TEST_SIZE];
  for(size_t i = 0; i < TEST_SIZE; i++) {
    int r;
    do {
      r = rand();
      r %= DIM;
    } while(is_present(bits_set, i, r));
    
    bits_set[i] = r;
    set_bv(&bv, r);
    assert(access_bv(&bv, r) == 1 && "The bit should be set");
  }

  assert(all_zeroes(&bv) == 0 && "Not all bits should be 0");

  for(size_t i = 0; i < TEST_SIZE; i++) {
    int r = bits_set[i];
    unset_bv(&bv, r);
    assert(access_bv(&bv, r) == 0 && "The bit should not be set");
  }
  printf("All the assertions are true");
  return 0;
}
