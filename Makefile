CC=clang
LD=clang

OBJS=$(shell find ./src -type f -iname '*.c' | sed 's-\./src-\./bin-g' | sed 's/\.c/\.o/g')

TARGET=junocam_rgb_proc.elf

INC=./include
BIN=./bin
SRC=./src

EXTERNAL_LIBRARY_CFLAGS=$(shell pkg-config --cflags sdl2 SDL2_image)
EXTERNAL_LIBRARY_LDFLAGS=$(shell pkg-config --libs sdl2 SDL2_image)
CFLAGS=-std=c23 -Wall -Wextra -Werror -I./include $(EXTERNAL_LIBRARY_CFLAGS)
LDFLAGS=$(EXTERNAL_LIBRARY_LDFLAGS)


test: clean build run

$(BIN) $(SRC) $(INC):
	mkdir -p $@

clean: $(BIN)
	rm -f $</*.{o,elf}

run: clean build
	$(BIN)/$(TARGET) $(ARGS)

build: clean $(BIN)/$(TARGET)

$(BIN)/$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

$(OBJS): $(BIN)/%.o : $(SRC)/%.c
	$(CC) -c $< $(CFLAGS) -o $@
