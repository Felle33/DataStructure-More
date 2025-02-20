#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define DA_INIT_CAP 32
#define UNDEFINED 0

#define DA_INIT(da)				\
  do {						\
    (da)->items = NULL;				\
    (da)->count = 0;				\
    (da)->capacity = UNDEFINED;				\
  } while(0)					\

// ATTENTION: not checking of overlflowing capacity constraint
#define DA_ADD(da, el)							\
  do {									\
    if ((da)->count + 1 > (da)->capacity) {				\
      (da)->capacity = (da)->capacity == UNDEFINED ? DA_INIT_CAP : (da)->capacity * 2; \
      (da)->items = realloc((da)->items, sizeof(int) * (da)->capacity);	\
      if ((da)->items == NULL) {					\
	exit(-1 && "You don't have enough RAM!\n");				\
      }									\
    }									\
    (da)->items[(da)->count] = el;					\
    (da)->count += 1;							\
  } while(0)								

#define DA_REMOVE(da, n)						\
  do {									\
    (da)->count -= (n);							\
    if((da)->count < (da)->capacity / 2 && (da)->capacity != DA_INIT_CAP) { \
      while ((da)->count < (da)->capacity / 2 && (da)->capacity != DA_INIT_CAP) { \
	(da)->capacity /= 2;						\
      }									\
      (da)->items = realloc((da)->items, sizeof(int) * (da)->capacity);	\
      if ((da)->items == NULL) {					\
	exit(-1 && "You don't have enough RAM!\n");			\
      }									\
    }									\
  } while(0)								\
   
typedef struct da_int {
  int* items;
  size_t count;
  size_t capacity;
} Da_int;

void insert_da(Da_int* da, int cnt) {
  for(int i = 0; i < cnt; i++) {
    DA_ADD(da, i);
  }
}

#if defined( __GNUC__) || defined(__clang__)
size_t leading_zeroes(size_t x) {
  // printf("You are compiling in GCC or clang!\n");
  if (sizeof(size_t) == 8) {
    // 64 bit systems
    return __builtin_clzll(x);
  }

  // 32 bit systems
  return __builtin_clz(x);
}
#else
size_t leading_zeroes(size_t x) {
  // implement via software
  size_t cnt = 0;
  for(int i = sizeof(size_t) * 8 - 1; i >= 0; i--) {
    if(x & (1 << i)) {
      break;
    }
    cnt += 1;
  }
  return cnt;
}
#endif // __GNUC__ && __clang__

// ATTENTION: not checking for overflow constraint in first_one
size_t pow2_exceed(size_t x) {
  size_t lead_zeroes = leading_zeroes(x);
  size_t first_one = sizeof(size_t) * 8 - lead_zeroes - 1; // position first one

  return (1 << first_one) == x ? x : 1 << (first_one + 1);
}

void test_initialization() {
  Da_int da;
  DA_INIT(&da);
  
  assert(da.items == 0 && "The items is not NULL");
  assert(da.count == 0 && "The count is not zero");
  assert(da.capacity == 0 && "The capacity is not zero");
}

void test_insertion() {
  Da_int da;
  size_t sz = 10000;
  
  DA_INIT(&da);

  insert_da(&da, sz);
  // now the da has sz elements
  assert(da.items != NULL && "The items should be not NULL");
  assert(da.count == sz && "The count should be 10000");
  // printf("Current capacity is %zu and pow2_exceed is %zu\n", da.capacity, pow2_exceed(da.capacity));
  assert(da.capacity == pow2_exceed(da.count) && "The capacity should be ceil_of_pow2(10000)");

  for(size_t i = 0; i < sz; i++) {
    assert(da.items[i] == i && "The item current value is not what expected\n");
  }
}

void test_insertion_remotion() {
  Da_int da;
  size_t sz = 10000;
  
  DA_INIT(&da);

  insert_da(&da, sz);
  // now the da has sz elements
  assert(da.items != NULL && "The items should be not NULL");
  assert(da.count == sz && "The count should be 1");
  // printf("Current capacity is %zu and pow2_exceed is %zu\n", da.capacity, pow2_exceed(da.capacity));
  assert(da.capacity == pow2_exceed(da.count) && "The capacity should be ceil_of_pow2(10000)");
  
  DA_REMOVE(&da, sz);
  assert(da.items != NULL && "The items should be not NULL");
  assert(da.count == 0 && "The count should be 0");
  assert(da.capacity == DA_INIT_CAP && "The capacity should be DA_INIT_CAP");
}

void test_insertion_remotion_control() {
  Da_int da;
  size_t sz = 10000;
  
  DA_INIT(&da);

  insert_da(&da, sz);
  // now the da has sz elements
  assert(da.items != NULL && "The items should be not NULL");
  assert(da.count == sz && "The count should be sz");
   assert(da.capacity == pow2_exceed(da.count) && "The capacity should be ceil_of_pow2(10000)");
  
  DA_REMOVE(&da, sz / 2);
  assert(da.items != NULL && "The items should be not NULL");
  assert(da.count == sz / 2 && "The count should be sz / 2");
  assert(da.capacity == pow2_exceed(da.count) && "The capacity should be ceil_of_pow2(sz / 2)");
  
  for(size_t i = 0; i < sz / 2; i++) {
    assert(da.items[i] == i && "The item current value is not what expected\n");
  }
}  

int main() {
  test_initialization();
  test_insertion();
  test_insertion_remotion();
  test_insertion_remotion_control();
  printf("All the assertion have been verified!\n");
  
  return 0;
}
