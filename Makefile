CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=`pkg-config --libs raylib`

SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,build/%.o,$(SRC))

all: $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o sorting $(LDFLAGS)
	@echo "sorting"

build/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) -o $@ $< -c $(CFLAGS)
	@echo $@

clean:
	@rm -rf sorting build/
	@echo "clean"

windows:
	$(CC) $(CFLAGS) $(SRC) -o sorting -static `pkg-config --libs raylib` -lgdi32 -lwinmm
