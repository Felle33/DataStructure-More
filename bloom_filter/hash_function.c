#include "hash_function.h"

unsigned long djb2(const unsigned char* str) {
  unsigned long hash = 5381;
  unsigned char c;

  while((c = *str++)) 
    hash = ((hash << 5) + hash) + c;
  
  return hash;
}

unsigned long sdbm(const unsigned char* str) {
  unsigned long hash = 0;
  unsigned char c;

  while((c = *str++))
    hash = c + (hash << 6) + (hash << 16) - hash;
  
  return hash;
}
