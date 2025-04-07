#include "skip_list.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
  skip_list* sl = init_skip_list();
  insert_skip_list(sl, 5);
  insert_skip_list(sl, 7);
  insert_skip_list(sl, 0);
  insert_skip_list(sl, 9);

  assert(search_skip_list(sl, -1) == false);
  assert(search_skip_list(sl, 0) == true);
  assert(search_skip_list(sl, 5) == true);
  assert(search_skip_list(sl, 8) == false);

  printf("All test have passed!");
  return 0;
}
