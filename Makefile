override CXXFLAGS += -std=c++11 -g -Wall -O2 -pipe -fPIE -Weffc++
INC               += -Iinclude -Iinclude/analyzers
LDFLAGS           := -lsmarttpl -lconfig++ -pthread -ldl -Wl,--export-dynamic
CC                := cc
CXX               := c++
CLANG_FORMAT      := clang-format

BINARY := scanic
INPUT_MODULES := inputs/input_znc.so
ANALYZERS := analyzers/linecount.so analyzers/smileycount.so analyzers/wordcount.so
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

.PHONY: format

format:
	find -name \*.cpp -exec $(CLANG_FORMAT) -style LLVM -i {} \;

.PHONY: clean

clean:
	rm -rf $(BINARY) build inputs analyzers
