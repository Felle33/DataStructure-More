//
// Created by tommy on 16/09/24.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, newSize);
    return result;
}

void initTable(Table* table) {
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void freeTable(Table* table) {
    FREE_ARRAY(Entry, table->entries, table->capacity);
    initTable(table);
}

uint32_t hashString(const char* key, int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

static ObjString* allocateString(char* chars, int length, uint32_t hash) {
    ObjString* entry = ALLOCATE_STRING;
    entry->chars = chars;
    entry->length = length;
    entry->hash = hash;
    return entry;
}

static Entry* findEntry(Table* table, ObjString* key) {
    Entry* tombstone = NULL;
    uint32_t index = key->hash % table->capacity;

    for (;;) {
        Entry *entry = &table->entries[index];

        if (entry->key == NULL) {
            if (entry->value == false) {
                // Empty entry
                return tombstone != NULL ? tombstone : entry;
            } else {
                // We found a tombstone
                if (tombstone == NULL) tombstone = entry;
            }

        } else if (entry->key->length == key->length && entry->key->hash == key->hash &&
            memcmp(entry->key->chars, key->chars, key->length) == 0) {
            // We found the key
            return entry;
        }

        index = (index + 1) % table->capacity;
    }
}

static void adjustCapacity(Table* table, int capacity) {
    Entry* entries = ALLOCATE(Entry, capacity);
    for(int i = 0; i < capacity; i++) {
        entries->key = NULL;
        entries->value = false;
    }

    table->count = 0;
    for(int i = 0; i < table->capacity; i++) {
        Entry* entry = &table->entries[i];
        if (entry->key == NULL) continue;

        Entry *dest = findEntry(table, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->count++;
    }

    FREE_ARRAY(Entry, table->entries, table->capacity);
    table->entries = entries;
    table->capacity = capacity;
}

bool tableGet(Table* table, ObjString* key, Value* value) {
    if (table->count == 0) return false;

    Entry *entry = findEntry(table, key);
    if (entry->key == NULL) return false;

    *value = entry->value;
    return true;
}

bool tableSet(Table* table, ObjString* key, Value value) {
    if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(table->capacity);
        adjustCapacity(table, capacity);
    }

    Entry *entry = findEntry(table, key);
    bool isNewKey = (entry->key == NULL);
    if (isNewKey && entry->value == false) table->count++;

    entry->key = key;
    entry->value = value;
    return isNewKey;
}

bool tableDelete(Table* table, ObjString* key) {
    if (table->count == 0) return false;

    Entry *entry = findEntry(table, key);
    if (entry->key == NULL) return false;

    entry->key = NULL;
    entry->value = true;
    return true;
}