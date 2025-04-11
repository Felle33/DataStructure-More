CC:=gcc
CFLAGS:=-Wall -Wextra -ggdb -O0
SRC:=main.c skip_list.c
INCLUDE_DIR:=.
TARGET:=skip_list
DEFINES:=

$(TARGET): build $(SRC)
	$(CC) $(CFLAGS) $(SRC) -I$(INCLUDE_DIR) $(DEFINES) -o build/$(TARGET)

build:
	mkdir -p ./build
