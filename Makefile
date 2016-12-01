#CXX specifies the compiler to use for C++
CXX = g++

#SRC specifies which files to compile as part of the project
SRC = $(wildcard ./src/*.cpp) $(wildcard ./src/model/*.cpp) $(wildcard ./src/view/*.cpp)

#OBJS specifies which files to compile as part of the project
OBJS = $(subst ./src, ./obj, $(SRC:.cpp=.o))

#OBJ_NAME specifies the name of our executable
OBJ_NAME = ./bin/Villa

ifeq ($(OS),Windows_NT)
	#COMPILER_FLAGS specifies the additional compilation options we're using
	# -w suppresses all warnings
	# -Wl,-subsystem,windows gets rid of the console window
	# -Wall enables all major warnings
	# -pedantic enables all warnings demanded by strict ISO C.
	# -O2 enables optimization during compilation
	# -std=c++11 enables support for C++ 11 features
	COMPILER_FLAGS = -Wall -Wl,-subsystem,windows -pedantic -g -O2 -std=c++11

	#INCLUDE_PATHS specifies the additional include paths we'll need
	INCLUDE_PATHS = -I./include -I./include/model -I./include/SDL2
	
	#LIBRARY_PATHS specifies the additional library paths we'll need
	LIBRARY_PATHS = -L./lib

	#LINKER_FLAGS specifies the libraries we're linking against
	LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	
	#RM specifies the tool for cleaning files
	RM = del /F $(subst /,\,$(OBJS) $(OBJ_NAME) $(OBJ_NAME).exe)
	
	#RM_TESTS specifies the tool for cleaning files
	RM_TESTS = del /F .\testrunner\gtest_main.a .\testrunner\obj\gtest_main.o .\testrunner\obj\gtest-all.o .\testrunner\testrunner .\testrunner\testrunner.exe
else
	#COMPILER_FLAGS specifies the additional compilation options we're using
	# -w suppresses all warnings
	# -Wl,-subsystem,windows gets rid of the console window
	# -Wall enables all major warnings
	# -pedantic enables all warnings demanded by strict ISO C.
	# -O2 enables optimization during compilation
	# -std=c++11 enables support for C++ 11 features
	COMPILER_FLAGS = -Wall -pedantic -O2 -std=c++11
	
	#INCLUDE_PATHS specifies the additional include paths we'll need
	INCLUDE_PATHS = -I./include -I./include/model
	
	#LIBRARY_PATHS specifies the additional library paths we'll need
	LIBRARY_PATHS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf $(shell sdl2-config --cflags)
	
	#RM specifies the tool for cleaning files
	RM = rm -f $(OBJS) $(OBJ_NAME) $(OBJ_NAME).exe
	
	#RM_TESTS specifies the tool for cleaning files
	RM_TESTS = rm -f ./testrunner/gtest_main.a ./testrunner/obj/gtest_main.o ./testrunner/obj/gtest-all.o ./testrunner/testrunner ./testrunner/testrunner.exe
endif

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
GTEST_CPPFLAGS += -isystem ./testrunner/include

# Flags passed to the C++ compiler.
GTEST_CXXFLAGS += -g -Wall -Wextra -pthread

# All tests produced by this Makefile.
TESTS = $(wildcard ./testrunner/src/*.cpp)

# All Google Test headers.
GTEST_HEADERS = ./testrunner/include/gtest/*.h \
                ./testrunner/include/gtest/internal/*.h

# All Google Test source files.
GTEST_SRCS_ = ./testrunner/src/gtest/*.cc ./testrunner/src/gtest/*.h $(GTEST_HEADERS)

#Define makefile targets
all : $(OBJS)
	$(CXX) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

tests : ./src/model/*.cpp ./src/PerlinNoise.cpp $(TESTS) ./testrunner/gtest_main.a
	$(CXX) $(INCLUDE_PATHS) $(GTEST_CPPFLAGS) $(GTEST_CXXFLAGS) -lpthread $^ -o ./testrunner/testrunner

./obj/%.o : ./src/%.cpp
	$(CXX) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -c -o $@ $<

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
./testrunner/obj/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(GTEST_CPPFLAGS) -I. $(GTEST_CXXFLAGS) -c \
            ./testrunner/src/gtest/gtest-all.cc -o ./testrunner/obj/gtest-all.o

./testrunner/obj/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(GTEST_CPPFLAGS) -I. $(GTEST_CXXFLAGS) -c \
            ./testrunner/src/gtest/gtest_main.cc -o ./testrunner/obj/gtest_main.o

./testrunner/gtest.a : ./testrunner/obj/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

./testrunner/gtest_main.a : ./testrunner/obj/gtest-all.o ./testrunner/obj/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

.PHONY : clean
clean :
	$(RM)

.PHONY : clean-tests
clean-tests :
	$(RM_TESTS)