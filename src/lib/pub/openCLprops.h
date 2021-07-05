#pragma once

#ifdef cl_khr_fp64
#define CL_DOUBLEPRECISION 
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

#elif defined(cl_amd_fp64)
#define CL_DOUBLEPRECISION 
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#endif

typedef double real;


const int WIDTH = 800;
const int HEIGHT = 800;
const int ITERATIONS = 2048;
const int NUMBER_OF_THREADS = 16;//
const int SEGMENT_SIZE = 200;//