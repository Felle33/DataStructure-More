CC:=gcc
CFLAGS:=-Wall -Wextra -ggdb
SRC:=src/test.c src/bloom_filter.c src/bit_vector.c src/hash_function.c
INCLUDE_DIR:=include
TARGET:=bloom_filter

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -I$(INCLUDE_DIR) -o build/$(TARGET)
