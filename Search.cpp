#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/opencl.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "SimpleCppTextFileHandler/file.hpp"
#include "Search.hpp"

Search::Search(){
	cl_int err;
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	cl::Platform plat;
	for(auto& p : platforms){
		std::string platver = p.getInfo<CL_PLATFORM_VERSION>();
		if(platver.find("OpenCL 3.") != std::string::npos){
			plat = p;
		}
	}

	if(plat() == 0){
		std::cerr<<"No OpenCL 3.0 platform found."<<std::endl;
		return(-1);
	}

	cl::Platform newP = cl::Platform::setDefault(plat);
	if(newP != plat){
		std::cerr<<"Error setting default platform."<<std::endl;
		return(-1);
	}

	kernel1 = readFile("kernel1.cl");
	kernel2 = readFile("kernel2.cl");

	std::vector<std::string> programStrings{kernel1, kernel2};

	SearchProgram = std::make_shared<cl::Program>(programStrings);
	try{
		SearchProgram->build("-cl-std=CL2.0");
	}catch(...){
		cl_int buildErr = CL_SUCCESS;
		auto buildInfo = SearchProgram->getBuildInfo<CL_PROGRAM_BUILD_LOG>(&buildErr);
		for(auto& pair : buildInfo){
			std::cerr<<pair.second<<std::endl;
		}
		return(1);
	}
}

Search::~Search(){

}

uint8_t checkSquareEmpty(const boardstate* currentBoardstate, uint8_t xpos, uint8_t ypos){//0 = true, 1 = black, 2 = white, 3 = off the board on x axes, 4 = off the board on y axes
	if(xpos > 8 || xpos < 1){
		return(3);
	}
	if(ypos > 8 || ypos < 1){
		return(4);
	}
	for(int i=0;i<32;i++){
		if(currentBoardstate[i].xpos == xpos && currentBoardstate[i].ypos == ypos)
			if(currentBoardstate[i].pieceType > 0 && currentBoardstate[i].pieceType <= 6){
				return(1);
			}else if(currentBoardstate[i].pieceType >= 7){
				return(2);
			}
	}
	return(0);
}

boardstate* copyBoardstate(const baordstate* currentBoardstate){
	boardstate* newBoardstate = new boardstate[32];
	std::memcpy(newBoardstate, currentBoardstate, sizeof(boardstate) * 32);
	return(newBoardstate);
}

std::optional<std::vector<boardstate*>> cpuSearchR(const boardstate* currentBoardstate, unsigned int currentSearchDepth){//Can't be used for an actual search due to it saving boardstates in memory
	std::vector<boardstate*> output;
	if(currentSearchDepth > maxCPUSearch){
		return(output);
	}
	for(int i=0;i<32;i++){
		uint8_t pieceType = currentBoardstate[i].pieceType;
		if(pieceType == 0){
			continue;
		}
		uint8_t xpos = currentBoardstate[i].xpos;
		uint8_t ypos = currentBoardstate[i].ypos;
		switch(pieceType){
			case 1:{//b pawn
				if(checkSquareEmpty(currentBoardstate, xpos, ypos+1) == 0){
					{
						boardstate* tempBoardstate = copyBoardstate(currentBoardstate);
						tempBoardstate[i].ypos++;
						output.push_back(tempBoardstate);
					}
					if(checkSquareEmpty(currentBoardstate, xpos, ypos+2) == 0){
						boardstate* tempBoardstate = copyBoardstate(currentBoardstate);
						tempBoardstate[i].ypos+=2;
						output.push_back(tempBoardstate);
					}
				}
				if(checkSquareEmpty(currentBoardstate, xpos+1, ypos
			}
			case 2://b knight
			case 3://b bushop
			case 4://b rook
			case 5://b queen
			case 6://b king
			case 7://w pawn
			case 8://w knight
			case 9://w bushop
			case 10://w rook
			case 11://w queen
			case 12://w king
		}
	}
}

std::vector<boardstate*> cpuSearch(const boardstate* currentBoardstate){
	auto searchDone = cpuSearchR(currentBoardstate, 1);
	if(searchDone){
		return(searchDone.value());
	}
	return(std::vector<boardstate*>());
}

void Search::go(){
	//so basically the stratagy here is to do a depth 1 or 2 search on the cpu, and then send those boards to OpenCL to do the following depth of nodes in parelel.
	std::vector<boardstate*> boardstates;//specifically boardstate[32]
	
	
	typedef struct {int* bar;} Foo;

	auto program2Kernel = cl::KernelFunctor<>(vectorAddProgram, "updateGlobal");
	program2Kernel(cl::EnqueueArgs(cl::NDRange(1)));//calling opencl func updateGlobal, on one instance(eg. not parelel)

	auto anSVMInt = cl::allocate_svm<int, cl::SVMTraitCoarse<>>();
	*anSVMInt = 5;
	cl::SVMAllocator<Foo, cl::SVMTraitCoarse<cl::SVMTraitReadOnly<>>> svmAllocReadOnly;
	auto fooPointer = cl::allocate_pointer<Foo>(svmAllocReadOnly);
	fooPointer->bar = anSVMInt.get();
	cl::SVMAllocator<int, cl::SVMTraitCoarse<>> svmAlloc;
	std::vector<int, cl::SVMAllocator<int, cl::SVMTraitCoarse<>>> inputA(numElements, 1, svmAlloc);
	cl::coarse_svm_vector<int> inputB(numElements, 2, svmAlloc);

	std::vector<int> output(numElements, 0x0);
	cl::Buffer outputBuffer(begin(output), end(output), false);
	cl::Pipe aPipe(sizeof(cl_int), numElements/2);

	cl::DeviceCommandQueue defaultDeviceQueue = cl::DeviceCommandQueue::makeDefault(cl::Context::getDefault(), cl::Device::getDefault());

	auto vectorAddKernel = cl::KernelFunctor<decltype(fooPointer)&, int*, cl::coarse_svm_vector<int>&, cl::Buffer, int, cl::Pipe&, cl::DeviceCommandQueue>(vectorAddProgram, "vectorAdd");

	vectorAddKernel.setSVMPointers(anSVMInt);//Ensure this is available even though it wasn't a parameter

	cl::enqueueUnmapSVM(anSVMInt);
	cl::enqueueUnmapSVM(fooPointer);
	cl::unmapSVM(inputB);
	//cl::unmapSVM(output2);

	cl_int error;
	vectorAddKernel(cl::EnqueueArgs(cl::NDRange(numElements/2), cl::NDRange(numElements/2)), fooPointer, inputA.data(), inputB, outputBuffer, 3, aPipe, defaultDeviceQueue, error);//Actually starts the opencl call

	cl::copy(outputBuffer, begin(output), end(output));

	//cl::mapSVM(output2);

	cl::Device d = cl::Device::getDefault();

	std::cout<<"Output: \n";
	for(int i=0;i<numElements;i++){//numelements = 32
		std::cout<<"\t"<<output[i]<<"\n";
	}
	std::cout<<std::endl;
	return(0);
}
