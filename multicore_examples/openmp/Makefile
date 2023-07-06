CFLAGS = -std=c11 -Wall -Wstrict-prototypes -Wredundant-decls -Wshadow \
				 -pedantic -pedantic -fno-strict-aliasing -D_XOPEN_SOURCE=700 -fopenmp -I./
LDFLAGS = -fopenmp
BIN = simple cache cache2 false-sharing false-sharing2 sum sum_reduce

all: $(BIN)
	
$(BIN): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(BIN)
	rm -f *.o 

