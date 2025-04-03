#include "bit_vector.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// Granularity of the chunks (in this case we use 8 bits, so 1 byte)
const size_t GRAN = sizeof(unsigned int);

// TODO: maybe there is the need to resize dynamically the array
void resize(bit_vector* bv, size_t size) {
  if(size % 8 == 0) {
    bv->size = size / GRAN;
  } else {
    bv->size = size / GRAN + 1;
  }

  bv->vec = malloc(sizeof(unsigned int) * bv->size);
  assert(bv->vec != NULL && "Cannot allocate bit vector! Not enough memory");
}

bit_vector init_bv(size_t size) {
  bit_vector bv = {0};
  resize(&bv, size);
  clear_all_bv(&bv);
  return bv;
}

bool access_bv(bit_vector* bv, size_t index) {
  size_t cell = index / GRAN;
  size_t offset = index % GRAN;
  return (bv->vec[cell] & (1 << offset)) != 0;
}

void set_all_bv(bit_vector* bv) {
  memset(bv->vec, 0xFF, sizeof(unsigned int) * bv->size);
}

void set_bv(bit_vector* bv, size_t index) {
  size_t cell = index / GRAN;
  size_t offset = index % GRAN;
  bv->vec[cell] |= (1 << offset);
}

void unset_bv(bit_vector* bv, size_t index) {
  size_t cell = index / GRAN;
  size_t offset = index % GRAN;
  bv->vec[cell] &= (~(1 << offset));
}

void clear_all_bv(bit_vector* bv) {
  memset(bv->vec, 0, sizeof(unsigned int) * bv->size);
}

void free_bv(bit_vector* bv) {
  free(bv->vec);
}

