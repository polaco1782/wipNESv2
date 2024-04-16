LIBS  = -lSDL2
CFLAGS += -O0 -g --std=gnu11

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,bin/%.o,$(wildcard *.c))

bin/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

znes: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o znes
