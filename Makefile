override CXXFLAGS += -std=c++11 -g -Wall -O2 -pipe -fPIE -Wl,--export-dynamic -Weffc++
INC               += -Iinclude -Iinclude/analyzers
LDFLAGS           := -lsmarttpl -pthread -ldl
CC                := cc
CXX               := c++

BINARY := scanic
INPUT_MODULES := inputs/input_znc.so
ANALYZERS := analyzers/linecount.so
DEPS := build/input.o build/analyzer.o build/generator.o

all: $(BINARY) $(INPUT_MODULES) $(ANALYZERS)

build:
	mkdir build

inputs:
	mkdir inputs

analyzers:
	mkdir analyzers

build/%.o: src/%.cpp include/%.h
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INC) -c $< -o $@

inputs/%.so: src/inputs/%.cpp inputs
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INC) -shared $< -o $@

analyzers/%.so: src/analyzers/%.cpp analyzers
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INC) -shared $< -o $@

$(BINARY): build $(DEPS) src/main.cpp
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INC) -o $(BINARY) src/main.cpp $(DEPS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -rf $(BINARY) build inputs analyzers
