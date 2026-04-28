CC=gcc
CFLAGS=-Wall -Wextra -std=gnu99
LDFLAGS=-lm

TARGET=search_index
OBJS=list.o commands.o trie.o heap.o system.o

build: $(TARGET)

$(TARGET): main.c $(OBJS)
	$(CC) $(CFLAGS) main.c $(OBJS) $(LDFLAGS) -o $(TARGET)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o *.exe indexare.out

.PHONY: build run clean