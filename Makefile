CXX = g++
CFLAGS = -g -Wall -Wextra -std=c++17 -O0
OBJ = \
			obj/add.o \
			obj/subtract.o \
			obj/multiply.o \
			obj/divide.o

HDRS = src/bignum.hpp

LINK =

.SUFFIXES:

obj/%.o: src/%.cpp $(HDRS)
	$(CXX) $(CFLAGS) $< -c -o $@

bignum_test: obj/main.o $(OBJ)
	$(CXX) $(CFLAGS) $^ -o $@ $(LINK)

clean:
	rm -rf obj
	rm -rf bignum_test
