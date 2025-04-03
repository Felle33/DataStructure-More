// In this implementation I have a different bit vector for each hash function
#include "bloom_filter.h"
#include <stdlib.h>

bloom_filter init_bloom_filter(size_t size, hash_function* hash_functions, size_t k) {
  bloom_filter bf;
  bf.bv = malloc(sizeof(bit_vector) * k);

  for(size_t i = 0; i < k; i++) {
    bf.bv[i] = init_bv(size);
  }
  
  bf.hash_functions = hash_functions;
  bf.k = k;
  return bf;
}

void add_item_bloom_filter(bloom_filter* bf, const void* data) {
  for(size_t i = 0; i < bf->k; i++) {
    set_bv(&bf->bv[i], bf->hash_functions[i](data));
  }
}
