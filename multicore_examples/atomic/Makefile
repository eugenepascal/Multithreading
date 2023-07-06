CFLAGS = -g -std=c11 -Wall -Wstrict-prototypes -Wredundant-decls -Wshadow -pedantic -pedantic -fno-strict-aliasing -D_XOPEN_SOURCE=700 -I./
CXXFLAGS = -g -std=c++17 -Wall -Wredundant-decls -Wshadow -pedantic -pedantic -fno-strict-aliasing
LDFLAGS = -lpthread
BIN = atomic
BIN_CPP = atomic_cpp

all: $(BIN) $(BIN_CPP)
	
$(BIN): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BIN_CPP): %: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(BIN) $(BIN_CPP)
	rm -f *.o 

