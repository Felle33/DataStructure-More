CC:=gcc
CFLAGS:=-Wall -Wextra -ggdb -O0
SRC:=persistent_segment_tree.c
INCLUDE_DIR:=.
TARGET:=persistent_segment_tree
DEFINES:=

$(TARGET): build $(SRC)
	$(CC) $(CFLAGS) $(SRC) -I$(INCLUDE_DIR) $(DEFINES) -o build/$(TARGET)

build:
	mkdir -p ./build
