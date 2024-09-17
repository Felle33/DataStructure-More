#include <stdio.h>
#include <string.h>

#include <hashmap.h>

ObjString *alloc(char *string) {
    int length = strlen(string);
    ObjString *key = ALLOCATE_STRING;
    key->chars = string;
    key->length = length;
    key->hash = hashString(string, length);
    return key;
}

int main(void) {
    Table table;
    Value value;
    ObjString *key;

    initTable(&table);
    key = alloc("ciao");
    tableSet(&table, key, true);

    if (tableGet(&table, key, &value)) {
        printf("The key-value pair is present! The value is %d\n", value);
    }

    key = alloc("bella");
    tableSet(&table, key, true);

    if (tableGet(&table, key, &value)) {
        printf("The key-value pair is present! The value is %d\n", value);
    }

    if(tableDelete(&table, key)) {
        printf("The key-value pair (key: %s) is now deleted!\n", key->chars);
    }

    return 0;
}
