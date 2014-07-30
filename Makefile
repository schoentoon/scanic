override CXXFLAGS += -std=c++11 -g -Wall -O2 -pipe -fPIE -Wl,--export-dynamic 
INC               += -Iinclude -Iinclude/analyzers
LDFLAGS           := -lvariant -ljson-c -pthread -ldl
CC                := cc
CXX               := c++

BINARY := scanic
INPUT_MODULES := inputs/input_znc.so
DEPS := build/input.o build/generator.o

all: $(BINARY)

build:
	mkdir build

inputs:
	mkdir inputs

build/%.o: src/%.cpp include/%.h build
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INC) -c $< -o $@

inputs/%.so: src/inputs/%.cpp inputs
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INC) -shared $< -o $@

$(BINARY): $(DEPS) src/main.cpp $(INPUT_MODULES)
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INC) -o $(BINARY) src/main.cpp $(DEPS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -rf $(BINARY) build inputs