CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

SOURCE = $(wildcard *.c)
OBJECT = $(wildcard *.o)

all: encode decode entropy

encode: encode.o io.o node.o huffman.o pq.o code.o stack.o
	$(CC) -o encode encode.o io.o node.o huffman.o pq.o code.o stack.o

decode: decode.o io.o node.o huffman.o pq.o code.o stack.o
	$(CC) -o decode decode.o io.o node.o huffman.o pq.o code.o stack.o

entropy: entropy.o
	$(CC) -o entropy entropy.o -lm
%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECT)

scan-build: clean
	scan-build make
format:
	clang-format -i -style=file *.c *.h
