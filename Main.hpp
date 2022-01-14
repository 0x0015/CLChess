#pragma once
#include <iostream>
#include "Board.hpp"
#include "Search.hpp"

class Main{
public:
	static inline Board gameBoard;
	static inline Search search;
	static int main(int argc, char** argv);
};
