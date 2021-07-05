#pragma once

#define __CL_ENABLE_EXCEPTIONS
#include <CL/opencl.h>
#include "cl.hpp"
#include "openCLprops.h"
//clang++ -lopencl -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -static -std=c++17 -L"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.2\lib\x64" -I"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.2\include" ./src/Fractal.cpp -o program.exe

#include <fstream>
#include <thread>
#include <mutex>
#include <iostream>
#include <streambuf>
#include <array>


	static std::atomic<int> workComplete;

class uniqueDevice {
private:
	///
	struct BaseDevice {
		virtual void init(int* fractal) = 0;
		virtual void compute(const real& x, const real& y, const real& scale) = 0;
	};
	///

	//
	class Gpu : public BaseDevice {
	private:
		cl::Platform mainPlatform;
		cl::Device mainDevice;
		cl::Context context;
		cl::CommandQueue queue;
		cl::Program gpuFractalProgram;
		cl::Kernel clKernel;
		cl::Buffer Bfractal;

		int* fractal;
	public:
		Gpu(): fractal(nullptr){}
		Gpu(int* fractal) { init( fractal ); }

		void init(int* fractal) override;
		void compute(const real& x, const real& y, const real& scale) override;
	};
	//
	struct WorkerThread
	{
		std::condition_variable cv_start;
		std::thread currentThread;
		std::mutex mux;

		bool bworking = false;
		int* fractal;

		real x;
		real y;
		real scale;
		int segmentNum;

		void start(const real& x, const real& y, const real& scale)
		{
			this->x = x;
			this->y = y;
			this->scale = scale;

			cv_start.notify_one();
		}

		void mandelbrotSetCreator();
	};
	//
	class Cpu : public BaseDevice {
	private:
		WorkerThread threads[NUMBER_OF_THREADS];

	public:
		Cpu(){}
		Cpu(int* fractal){ init(fractal); }

		~Cpu()
		{
			
		}

		void init(int* fractal) override;
		void compute(const real& x, const real& y, const real& scale) override;
	};
	//

public:
	uniqueDevice(int* fractal) {
		curDevice = GPU;
		devices[GPU]->init(fractal);
		devices[CPU]->init(fractal);
	}
	uniqueDevice(const uniqueDevice& obj) = delete;
	uniqueDevice(uniqueDevice&& obj) = delete;

	~uniqueDevice() {}

	enum Device {
		GPU = 0,
		CPU
	};

	Device getCurrentDevice();
	void setCurrentDevice(Device& dev);
	void changeDevice();

	void computeFractal(real x, real y, real scale, int* fractal) {
		devices[curDevice]->compute(x, y, scale);
	}

private:
	Device curDevice;
	BaseDevice* devices[2] = { new Gpu(), new Cpu() };
};