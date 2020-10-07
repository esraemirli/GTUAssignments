CXX = gcc
CXXFLAGS = -Wall -Wextra -pedantic -g  -I .
LDFLAGS = 
LBLIBS = -lm 

SRC = *.c
OBJ = $(SRC:.cc=.o)
EXEC = program

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LBLIBS) -pthread

clean:
	rm *.o program
