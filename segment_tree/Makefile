CC:=gcc
CFLAGS:=-Wall -Wextra -ggdb -O0
SRC:=main.c segment_tree.c
INCLUDE_DIR:=.
TARGET:=segment_tree
DEFINES:=

$(TARGET): build $(SRC)
	$(CC) $(CFLAGS) $(SRC) -I$(INCLUDE_DIR) $(DEFINES) -o build/$(TARGET)

build:
	mkdir -p ./build
