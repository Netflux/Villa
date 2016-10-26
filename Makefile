#CXX specifies the compiler to use for C++
CXX = g++

#SRC specifies which files to compile as part of the project
SRC = $(wildcard ./src/*.cpp) $(wildcard ./src/model/*.cpp) $(wildcard ./src/view/*.cpp)

#OBJS specifies which files to compile as part of the project
OBJS = $(subst ./src, ./obj, $(SRC:.cpp=.o))

#OBJ_NAME specifies the name of our executable
OBJ_NAME = ./bin/Villa
	
#RM specifies the tool for cleaning files
RM = rm -f $(OBJS) $(OBJ_NAME) $(OBJ_NAME).exe
	
#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
# -Wall enables all major warnings
# -pedantic enables all warnings demanded by strict ISO C.
# -std=c++11 enables support for C++ 11 features
COMPILER_FLAGS = -Wall -pedantic -std=c++11

ifeq ($(OS),Windows_NT)
	#INCLUDE_PATHS specifies the additional include paths we'll need
	INCLUDE_PATHS = -I./include -I./include/model -I./include/SDL2
	
	#LIBRARY_PATHS specifies the additional library paths we'll need
	LIBRARY_PATHS = -L./lib

	#LINKER_FLAGS specifies the libraries we're linking against
	LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
else
	#INCLUDE_PATHS specifies the additional include paths we'll need
	INCLUDE_PATHS = -I./include -I./include/model
	
	#LIBRARY_PATHS specifies the additional library paths we'll need
	LIBRARY_PATHS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf $(shell sdl2-config --cflags)
endif

#Define makefile targets
all : $(OBJS)
	$(CXX) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

./obj/%.o : ./src/%.cpp
	$(CXX) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -c -o $@ $<

.PHONY : clean
clean :
	$(RM)