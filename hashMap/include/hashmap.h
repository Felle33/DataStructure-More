//
// Created by tommy on 16/09/24.
//
#include <stdbool.h>
#include <stdint.h>

#ifndef DS_HASHMAP_H
#define DS_HASHMAP_H

#define TABLE_MAX_LOAD 0.75

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*) reallocate(pointer, sizeof(type) * (oldCount), \
    sizeof(type) * newCount)

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

#define ALLOCATE(type, capacity) \
    (type*) reallocate(NULL, 0, sizeof(type) * capacity)

#define ALLOCATE_STRING (ObjString*) reallocate(NULL, 0, sizeof(ObjString))

typedef bool Value;

typedef struct {
    int length;
    uint32_t hash;
    char* chars;
} ObjString;

typedef struct {
    ObjString* key;
    Value value;
} Entry;

typedef struct {
    int count;
    int capacity;
    Entry* entries;
} Table;

void* reallocate(void* pointer, size_t oldSize, size_t newSize);
uint32_t hashString(const char* key, int length);
void initTable(Table* table);
void freeTable(Table* table);
bool tableGet(Table* table, ObjString* key, Value* value);
bool tableSet(Table* table, ObjString* key, Value value);
bool tableDelete(Table* table, ObjString* key);

#endif
