override CXXFLAGS += -std=c++11 -g -Wall -O2 -pipe
INC               += -Iinclude -Iinclude/analyzers
LDFLAGS           := -lvariant -ljson-c
CC                := cc
CXX               := c++

BINARY := scanic
DEPS := build/input.o build/input_znc.o build/generator.o

all: $(BINARY)

build:
	mkdir build

build/%.o: src/%.cpp include/%.h
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INC) -c $< -o $@

$(BINARY): build $(DEPS) src/main.cpp
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INC) -o $(BINARY) src/main.cpp $(DEPS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -rf $(BINARY) build