#ifndef _HASH_FUNCTION_
#define _HASH_FUNCTION_

typedef unsigned long (*hash_function)(const void* data);

unsigned long djb2(const unsigned char* str);
unsigned long sdbm(const unsigned char* str);

#endif // _HASH_FUNCTION_
