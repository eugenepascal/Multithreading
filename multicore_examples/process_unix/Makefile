CFLAGS = -std=c11 -Wall -Wstrict-prototypes -Wredundant-decls -Wshadow -pedantic -pedantic -fno-strict-aliasing -D_XOPEN_SOURCE=700 -O2 -I./ -g
LDFLAGS = -lpthread -lrt
BIN = fork semaphore_posix message_queue_posix shared_memory_posix tube

all: $(BIN)
	
$(BIN): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(BIN)
	rm -f *.o 

