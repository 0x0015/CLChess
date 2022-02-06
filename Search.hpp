#pragma once
#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/cl2.hpp>
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
	unsigned int maxCPUSearch = 1;
	std::shared_ptr<cl::Program> SearchProgram;
//	std::vector<uint8_t*> cpuSearchR(const uint8_t* currentBoardstate, unsigned int currentSearchDepth, bool bmove);
	std::vector<uint8_t*> cpuSearch();
	Search();
	~Search();
	bool infinite;
	void go();
};
