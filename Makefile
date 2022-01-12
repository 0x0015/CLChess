OBJS	= main.cpp.o\
	  UCI.cpp.o \
	  Board.cpp.o \
	  Move.cpp.o \
	  SimpleCppTextFileHandler/file.cpp.o \
	  SimpleCppTextFileHandler/fileManipulation.cpp.o \
	  SimpleCppTextFileHandler/getExecutablePath.cpp.o 

OUT	= main
CXX	= g++
CC      = gcc
BUILD_CXX_FLAGS	 = -Wall -std=c++20
BULID_CC_FLAGS   =
LINK_OPTS	 = -lOpenCL -lstdc++fs

all: $(OBJS)
	$(CXX) -g $(OBJS) -o $(OUT) $(LINK_OPTS)


%.cpp.o: %.cpp
	$(CXX) $< $(BUILD_CXX_FLAGS) $(LINK_OPTS) -g -c -o $@

%.c.o: %.c
	$(CXX) $< $(BUILD_CXX_FLAGS) $(LINK_OPTS) -g -c -o $@

clean:
	rm -f $(OBJS) $(OUT)

