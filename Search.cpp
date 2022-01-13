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

void Search::go(){
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
