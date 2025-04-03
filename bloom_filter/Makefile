CC:=gcc
CFLAGS:=-Wall -Wextra -ggdb
SRC:=test.c bloom_filter.c bit_vector.c hash_function.c
TARGET:=bloom_filter

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -I. -o build/$(TARGET)
