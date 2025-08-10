CXX = g++
CXXFLAGS = -Iinclude -std=c++11 -Wall -Werror
LDFLAGS = -Llib -ldaas -lpthread -lbluetooth


SRC_COMMON = src/daas_chat.cpp

all: nodo_a nodo_b

nodo_a: main/main_a.cpp src/nodo_a.cpp $(SRC_COMMON)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

nodo_b: main/main_b.cpp src/nodo_b.cpp $(SRC_COMMON)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f nodo_a nodo_b
