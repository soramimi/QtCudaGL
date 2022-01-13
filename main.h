#ifndef MAIN_H
#define MAIN_H

#include "libTestCUDA/libtestcuda.h"

struct ApplicationGlobal {
	CUDAIMAGE_API const *cuda = nullptr;
};

extern ApplicationGlobal *global;

#endif // MAIN_H
