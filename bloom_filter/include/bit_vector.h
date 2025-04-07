#ifndef _BIT_VECTOR_
#define _BIT_VECTOR_

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  unsigned int* vec;
  size_t size;
} bit_vector;

// Initialize the bit vector passing as parameter the number of bits
bit_vector init_bv(size_t size);

// Access the bit vector
bool access_bv(bit_vector* bv, size_t index);

void set_all_bv(bit_vector* bv);

// Set the bit in position index in the bit vector
void set_bv(bit_vector* bv, size_t index);

// Unset the bit in position index in the bit vector
void unset_bv(bit_vector* bv, size_t index);

// Clear all the bits in the bit vector
void clear_all_bv(bit_vector* bv);

// Free the bit vector
void free_bv(bit_vector* bv);

#endif // _BIT_VECTOR_
