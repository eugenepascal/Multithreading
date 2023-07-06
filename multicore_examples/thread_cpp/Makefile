# For clang compilation, uncomment -stdlib=libc++ in CXXFLAGS
CXXFLAGS = -g -std=c++20 -Wall -Wredundant-decls -Wshadow -pedantic \
					 -fno-strict-aliasing
# 				 -stdlib=libc++
LDFLAGS = -lpthread
BIN = create create2 detach mutex mutex2 mutex3 mutex_guard condition condition2 \
			rwlock rwlock2 mutex_deadlock mutex_livelock once once2 tls barrier barrier2 semaphore

all: $(BIN)
	
$(BIN): %: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(BIN)
	rm -f *.o 

