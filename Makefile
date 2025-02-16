CC = gcc
CFLAGS = -Wall -Wextra -g -Wno-error=format

SRC = main.c cache.c
OBJ = $(SRC:.c=.o)
EXEC = cache

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

main.o: main.c cache.h
	$(CC) $(CFLAGS) -c main.c -o main.o

cache.o: cache.c cache.h
	$(CC) $(CFLAGS) -c cache.c -o cache.o

clean:
	rm -f $(OBJ) $(EXEC)
