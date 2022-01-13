#pragma once
#include <iostream>
#include <optional>
#include <memory>

class Search{
public:
	std::optional<unsigned int> depth;
	std::optional<unsigned int> wtime;
	std::optional<unsigned int> btime;
	std::optional<unsigned int> movetime;
	std::string kernel1;
	std::string kernel2;
	unsigned int numElements;
	std::shared_ptr<cl::Program> SearchProgram;
	Search();
	~Search();
	bool infinite;
	void go();
};
