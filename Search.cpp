#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/opencl.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "SimpleCppTextFileHandler/file.hpp"
#include "Search.hpp"
#include "Main.hpp"

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
		return;
	}

	cl::Platform newP = cl::Platform::setDefault(plat);
	if(newP != plat){
		std::cerr<<"Error setting default platform."<<std::endl;
		return;
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
		return;
	}
}

Search::~Search(){

}

bool isValidPosition(uint8_t xpos, uint8_t ypos){
	return(1<=xpos && xpos <= 8 && 1 <= ypos && ypos <= 8);
}

uint8_t* copyBoardstate(const uint8_t* currentBoardstate){
	uint8_t* newBoardstate = new uint8_t[32];
	std::memcpy(newBoardstate, currentBoardstate, 32);
	return(newBoardstate);
}

std::vector<uint8_t*> Search::cpuSearchR(const uint8_t* currentBoardstate, unsigned int currentSearchDepth, bool bmove){//Can't be used for an actual search due to it saving boardstates in memory
	std::vector<uint8_t*> output;
	if(currentSearchDepth > maxCPUSearch){
		return(output);
	}
	for(uint8_t x = 1;x<=8;x++){
		for(uint8_t y=1;y<=8;y++){
			uint8_t pieceType = getBoardPiece(currentBoardstate, x, y);
			if(pieceType == 0){
				continue;
			}
			if(bmove){
				switch(pieceType){
					case 1:{//b pawn
						if(isValidPosition(x, y-1) && getBoardPiece(currentBoardstate, x, y-1) == 0){
							{
								uint8_t* tempBoardstate = copyBoardstate(currentBoardstate);
								setBoardPiece(tempBoardstate, x, y, 0);
								setBoardPiece(tempBoardstate, x, y-1, 1);
								output.push_back(tempBoardstate);
							}
							if(isValidPosition(x, y-1) && getBoardPiece(currentBoardstate, x, y-2) == 0){
								uint8_t* tempBoardstate = copyBoardstate(currentBoardstate);
								setBoardPiece(tempBoardstate, x, y, 0);
								setBoardPiece(tempBoardstate, x, y-2, 1);
								output.push_back(tempBoardstate);
							}
						}
						if(isValidPosition(x-1, y-1) && getBoardPiece(currentBoardstate, x-1, y-1) >= 7){	
								uint8_t* tempBoardstate = copyBoardstate(currentBoardstate);
								setBoardPiece(tempBoardstate, x, y, 0);
								setBoardPiece(tempBoardstate, x-1, y-1, 1);
								output.push_back(tempBoardstate);
						}
						if(isValidPosition(x+1, y-1) && getBoardPiece(currentBoardstate, x+1, y-1) >= 7){	
								uint8_t* tempBoardstate = copyBoardstate(currentBoardstate);
								setBoardPiece(tempBoardstate, x, y, 0);
								setBoardPiece(tempBoardstate, x+1, y-1, 1);
								output.push_back(tempBoardstate);
						}
						//not worrying about enpassant right now
						break;
					}
					case 2:{//b knight
						if(isValidPosition(x+2, y+1)){
							uint8_t currentPiece = getBoardPiece(currentBoardstate, x+2, y+1);
							if(currentBoardstate == 0 || currentBoardstate >= 7){
								uint8_t* tempBoardstate = copyBoardstate(currentBoardstate);
								setBoardPiece(tempBoardstate, x, y, 0);
								setBoardPiece(tempBoardstate, x+2, y+1, 2);
								output.push_back(tempBoardstate);
							}
						}
						if(isValidPosition(x+2, y-1)){
							uint8_t currentPiece = getBoardPiece(currentBoardstate, x+2, y-1);
							if(currentBoardstate == 0 || currentBoardstate >= 7){
								uint8_t* tempBoardstate = copyBoardstate(currentBoardstate);
								setBoardPiece(tempBoardstate, x, y, 0);
								setBoardPiece(tempBoardstate, x+2, y-1, 2);
								output.push_back(tempBoardstate);
							}
						}
						if(isValidPosition(x-2, y+1)){
							uint8_t currentPiece = getBoardPiece(currentBoardstate, x-2, y+1);
							if(currentBoardstate == 0 || currentBoardstate >= 7){
								uint8_t* tempBoardstate = copyBoardstate(currentBoardstate);
								setBoardPiece(tempBoardstate, x, y, 0);
								setBoardPiece(tempBoardstate, x-2, y+1, 2);
								output.push_back(tempBoardstate);
							}
						}
						if(isValidPosition(x-2, y-1)){
							uint8_t currentPiece = getBoardPiece(currentBoardstate, x-2, y-1);
							if(currentBoardstate == 0 || currentBoardstate >= 7){
								uint8_t* tempBoardstate = copyBoardstate(currentBoardstate);
								setBoardPiece(tempBoardstate, x, y, 0);
								setBoardPiece(tempBoardstate, x-2, y-1, 2);
								output.push_back(tempBoardstate);
							}
						}
						if(isValidPosition(x+1, y+2)){
							uint8_t currentPiece = getBoardPiece(currentBoardstate, x+1, y+2);
							if(currentBoardstate == 0 || currentBoardstate >= 7){
								uint8_t* tempBoardstate = copyBoardstate(currentBoardstate);
								setBoardPiece(tempBoardstate, x, y, 0);
								setBoardPiece(tempBoardstate, x+1, y+2, 2);
								output.push_back(tempBoardstate);
							}
						}
						if(isValidPosition(x+1, y-2)){
							uint8_t currentPiece = getBoardPiece(currentBoardstate, x+1, y-2);
							if(currentBoardstate == 0 || currentBoardstate >= 7){
								uint8_t* tempBoardstate = copyBoardstate(currentBoardstate);
								setBoardPiece(tempBoardstate, x, y, 0);
								setBoardPiece(tempBoardstate, x+1, y-2, 2);
								output.push_back(tempBoardstate);
							}
						}
						if(isValidPosition(x-1, y+2)){
							uint8_t currentPiece = getBoardPiece(currentBoardstate, x-1, y+2);
							if(currentBoardstate == 0 || currentBoardstate >= 7){
								uint8_t* tempBoardstate = copyBoardstate(currentBoardstate);
								setBoardPiece(tempBoardstate, x, y, 0);
								setBoardPiece(tempBoardstate, x-1, y+2, 2);
								output.push_back(tempBoardstate);
							}
						}
						if(isValidPosition(x-1, y-2)){
							uint8_t currentPiece = getBoardPiece(currentBoardstate, x-1, y-2);
							if(currentBoardstate == 0 || currentBoardstate >= 7){
								uint8_t* tempBoardstate = copyBoardstate(currentBoardstate);
								setBoardPiece(tempBoardstate, x, y, 0);
								setBoardPiece(tempBoardstate, x-1, y-2, 2);
								output.push_back(tempBoardstate);
							}
						}
						break;
					}
					case 3:{//b bushop
						
						break;
					}
					case 4:{//b rook
						
						break;
					}
					case 5:{//b queen
						break;
					}
					case 6:{//b king
						break;
					}
				}
			}else{
				switch(pieceType){
					case 7:{//w pawn 
						break;
					}   
					case 8:{//w knight
						break;
					}   
					case 9:{//w bushop
						break;
					}   
					case 10:{//w rook
						break;
					}   
					case 11:{//w queen
						break;
					}   
					case 12:{//w king
						break;
					}   
				}
			}
			
		}
	}
	return(output);
}

std::vector<uint8_t*> Search::cpuSearch(){
	return(cpuSearchR(Main::gameBoard.Position, 1, Main::gameBoard.bMove));
}

void Search::go(){
	//so basically the stratagy here is to do a depth 1 or 2 search on the cpu, and then send those boards to OpenCL to do the following depth of nodes in parelel.
	std::vector<uint8_t*> boardstates;//specifically boardstate[32]
	
	
	typedef struct {int* bar;} Foo;

	auto program2Kernel = cl::KernelFunctor<>(*SearchProgram, "updateGlobal");
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

	auto vectorAddKernel = cl::KernelFunctor<decltype(fooPointer)&, int*, cl::coarse_svm_vector<int>&, cl::Buffer, int, cl::Pipe&, cl::DeviceCommandQueue>(*SearchProgram, "vectorAdd");

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
}
