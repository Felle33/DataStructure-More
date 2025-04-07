#ifndef _BLOOM_FILTER_
#define _BLOOM_FILTER_

#include "bit_vector.h"
#include "hash_function.h"
#include <stdbool.h>

typedef struct {
  // Array of bit vectors, so I can implement also other bloom filter algorithms
  bit_vector* bv;

  // Hash functions
  hash_function* hash_functions;
  
  // Number of hash functions
  size_t k;
} bloom_filter;

// Initialize bloom filter
bloom_filter init_bloom_filter(size_t size, hash_function* hash_functions, size_t k);

// Add an element to the bloom filter
void add_item_bloom_filter(bloom_filter* bf, const void* data);

// Check if an item is in the bloom filter
bool check_item_bloom_filter(bloom_filter* bf, const void* data);

// Free the memory occupied by the bloom filter
void free_bloom_filter(bloom_filter* bf);

#endif // _BLOOM_FILTER_
