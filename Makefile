#CXX specifies the compiler to use for C++
CXX = g++

#SRC specifies which files to compile as part of the project
SRC = $(wildcard ./src/*.cpp)

#OBJS specifies which files to compile as part of the project
OBJS = $(subst ./src, ./obj, $(SRC:.cpp=.o))

#OBJ_NAME specifies the name of our executable
OBJ_NAME = ./bin/Villa

ifeq ($(OS),Windows_NT)
	#INCLUDE_PATHS specifies the additional include paths we'll need
	INCLUDE_PATHS = -I./include -I./include/SDL2
	
	#LIBRARY_PATHS specifies the additional library paths we'll need
	LIBRARY_PATHS = -L./lib
	
	#COMPILER_FLAGS specifies the additional compilation options we're using
	# -w suppresses all warnings
	# -Wl,-subsystem,windows gets rid of the console window
	# -Wall enables all major warnings
	# -pedantic enables all warnings demanded by strict ISO C.
	COMPILER_FLAGS = -Wall -pedantic -std=c++11

	#LINKER_FLAGS specifies the libraries we're linking against
	LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	
	#RM specifies the tool for cleaning files
	RM = del /F $(subst /,\,$(OBJS) $(OBJ_NAME) $(OBJ_NAME).exe)
else
	#INCLUDE_PATHS specifies the additional include paths we'll need
	INCLUDE_PATHS = -I./include
	
	#LIBRARY_PATHS specifies the additional library paths we'll need
	LIBRARY_PATHS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf $(shell sdl2-config --cflags)
	
	#COMPILER_FLAGS specifies the additional compilation options we're using
	# -w suppresses all warnings
	# -Wl,-subsystem,windows gets rid of the console window
	# -Wall enables all major warnings
	# -pedantic enables all warnings demanded by strict ISO C.
	COMPILER_FLAGS = -Wall -pedantic
	
	#RM specifies the tool for cleaning files
	RM = rm -f $(OBJS) $(OBJ_NAME) $(OBJ_NAME).exe
endif

#Define makefile targets
all : $(OBJS)
	$(CXX) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

./obj/%.o : ./src/%.cpp
	$(CXX) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -c -o $@ $<

.PHONY : clean
clean :
	$(RM)