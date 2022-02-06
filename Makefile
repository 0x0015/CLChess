OBJS	= main.cpp.o\
	  Main.cpp.o \
	  UCI.cpp.o \
	  board.c.o \
	  bitboard.c.o \
	  Move.cpp.o \
	  Search.cpp.o \
	  SimpleCppTextFileHandler/file.cpp.o \
	  SimpleCppTextFileHandler/fileManipulation.cpp.o \
	  SimpleCppTextFileHandler/getExecutablePath.cpp.o 

OUT	= main
CXX	= g++
CC = gcc
BUILD_CXX_FLAGS	 = -Wall -std=c++17 -g
BULID_CC_FLAGS   =
LINK_OPTS	 = -lOpenCL -lstdc++fs

all: $(OBJS)
	$(CXX) -g $(OBJS) -o $(OUT) $(LINK_OPTS)


%.cpp.o: %.cpp
	$(CXX) $< $(BUILD_CXX_FLAGS) $(LINK_OPTS) -g -c -o $@

%.c.o: %.c
	$(CC) $< $(BUILD_CXX_FLAGS) $(LINK_OPTS) -g -c -o $@

clean:
	rm -f $(OBJS) $(OUT)

