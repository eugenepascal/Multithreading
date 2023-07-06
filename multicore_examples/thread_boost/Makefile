CXXFLAGS = -std=c++17 -Wall -Wredundant-decls -Wshadow -pedantic -fno-strict-aliasing
LDFLAGS = -lboost_thread -lboost_system -lboost_chrono -lpthread
BIN = create create2 detach mutex mutex2 mutex3 mutex_guard condition \
			condition2 rwlock rwlock2 barrier barrier2 once tls semaphore

all: $(BIN)
	
$(BIN): %: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(BIN)
	rm -f *.o 

