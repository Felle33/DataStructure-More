#include <stdio.h>
#include "bloom_filter.h"
#include "hash_function.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int main(void) {
  hash_function hash_functions[] = {(hash_function)djb2, (hash_function)sdbm};
  bloom_filter bf = init_bloom_filter(128, hash_functions, ARRAY_SIZE(hash_functions));

  free_bloom_filter(&bf);
  return 0;
}
