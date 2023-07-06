CFLAGS = -std=c18 -Wall -Wstrict-prototypes -Wredundant-decls -Wshadow -pedantic -fno-strict-aliasing -D_XOPEN_SOURCE=700 -I./
LDFLAGS = -lpthread
BIN = create create2 detach condition mutex mutex2 mutex3 \
			mutex_deadlock mutex_livelock once tls tls2

all: $(BIN)
	
$(BIN): %: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(BIN)
	rm -f *.o 

