CXXFLAGS = -Wall -Wextra -pedantic -std=c++0x -MP -MMD
LINKS = -lncurses
EXEC = ../todo

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o) 

all: $(EXEC)

$(OBJ): %.o : %.cpp

$(OBJ):
	$(CXX) $(CXXFLAGS) -c $< -o $(<:.cpp=.o)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJ) $(LINKS)

-include $(SRC:.cpp=.d)
