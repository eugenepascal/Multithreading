CFLAGS = -std=c11 -Wall -Wstrict-prototypes -Wredundant-decls -Wshadow \
				 -pedantic -pedantic -fno-strict-aliasing -D_XOPEN_SOURCE=700 -I./ \
				 -DCL_TARGET_OPENCL_VERSION=200
LDFLAGS = -lOpenCL
BIN = hello info 

all: $(BIN)
	
$(BIN): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(BIN)
	rm -f *.o 

