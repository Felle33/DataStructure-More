#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define INIT_DA(da)    \
  do {                 \
    da.items = NULL;  \
    da.count = 0;     \
    da.capacity = 0;  \
  } while(0)           \

typedef struct da_int {
  int* items;
  size_t count;
  size_t capacity;
} Da_int;

int main() {
  Da_int my_da;
  INIT_DA(my_da);
  
  assert(my_da.items == 0 && "The items is not NULL");
  assert(my_da.count == 0 && "The count is not zero");
  assert(my_da.capacity == 0 && "The capacity is not zero");

  printf("All the assertion have been verified!\n");
  
  return 0;
}
