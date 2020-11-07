CXX=clang++
SRC=tests/static_any.cpp
CXXFLAGS += -std=c++2a -W -Wall -Wextra -I inc/ -O3
OBJ=$(SRC:.cpp=.o)

test: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $?

clean:
	rm -f $(OBJ) test
