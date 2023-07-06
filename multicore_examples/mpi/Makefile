CC = mpicc
CFLAGS = -g -std=c11 -Wall -Wstrict-prototypes -Wredundant-decls -Wshadow \
				 -pedantic -pedantic -fno-strict-aliasing -D_XOPEN_SOURCE=700 -fopenmp -I./
LDFLAGS = -lmpi -fopenmp
BIN = simple simple-openmp send-recv async-send-recv sum sum_reduce

all: $(BIN)
	
$(BIN): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(BIN)
	rm -f *.o 

