CFLAGS = -std=c18 -Wall -Wstrict-prototypes -Wredundant-decls -Wshadow -pedantic -fno-strict-aliasing -D_XOPEN_SOURCE=700 -I./
LDFLAGS = -lhwloc
BIN = topo_bind 

all: $(BIN)
	
$(BIN): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(BIN)
	rm -f *.o 

