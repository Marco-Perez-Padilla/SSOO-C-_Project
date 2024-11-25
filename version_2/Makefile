CXX = g++
CXXFLAGS = -std=c++23 -Wall -g

SOURCES = $(wildcard *.cc)
OBJ = $(SOURCES:.cc=.o)

EXECUTABLE = docserver

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(EXECUTABLE)
