#define __CL_ENABLE_EXCEPTIONS
#define OLC_PGE_APPLICATION

#include <CL/opencl.h>
#include "lib/pub/openCLprops.h"
#include "lib/pub/cl.hpp"
#include "lib/pub/deviceApi.h"
#include "lib/pub/olcPixelGameEngine.h"


#include <fstream>
#include <thread>
#include <mutex>
#include <iostream>
#include <streambuf>


real scale = 0.005;
int fractal[WIDTH * HEIGHT];
olc::vd2d offset(0, 0);//
uniqueDevice device(fractal);
olc::vd2d beforeZooming, afterZooming;

/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Window : public olc::PixelGameEngine
{
public:
	Window() {}

	bool OnUserCreate()override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime)override
	{
	

		if (GetKey(olc::Key::W).bHeld) offset.y += 100 * scale * fElapsedTime;
		if (GetKey(olc::Key::A).bHeld) offset.x -= 100 * scale * fElapsedTime;
		if (GetKey(olc::Key::S).bHeld) offset.y -= 100 * scale * fElapsedTime;
		if (GetKey(olc::Key::D).bHeld) offset.x += 100 * scale * fElapsedTime;

		if (GetKey(olc::Key::UP).bHeld) 
		{ 
			beforeZooming = offset + olc::vd2d{ WIDTH / 2 * (double)scale, -WIDTH / 2 * (double)scale };
			scale *= 0.99; 
			afterZooming = offset + olc::vd2d{ WIDTH / 2 * (double)scale, -WIDTH / 2 * (double)scale };

			offset += beforeZooming - afterZooming;
		}
		if (GetKey(olc::Key::DOWN).bHeld) 
		{ 
			beforeZooming = offset + olc::vd2d{ WIDTH / 2 * (double)scale, -WIDTH / 2 * (double)scale };
			scale *= 1.01;
			afterZooming = offset + olc::vd2d{ WIDTH / 2 * (double)scale, -WIDTH / 2 * (double)scale };

			offset += beforeZooming - afterZooming;
		}

		if (GetKey(olc::Key::C).bPressed)device.changeDevice();

		device.computeFractal(offset.x, offset.y, scale, fractal);

		for (int x = 0; x < WIDTH; x++)
		{
			for (int y = 0; y < HEIGHT; y++)
			{
				real f = fractal[y * WIDTH + x];
				Draw(x, y, olc::PixelF(f / ITERATIONS, f / ITERATIONS, f / ITERATIONS));

			}
		}
		return true;
	}
};

int main()
{
	Window window;
	window.Construct(WIDTH, HEIGHT, 1, 1);
	window.Start();

	system("PAUSE");
}