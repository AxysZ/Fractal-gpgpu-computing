#include "pub/deviceApi.h"

void uniqueDevice::Gpu::init(int* fractal) {
	using namespace std;
	this->fractal = fractal;

	vector<cl::Platform> avaliablePlatforms;
	vector<cl::Device> avaliableDevices;
	int num;

	cl::Platform::get(&avaliablePlatforms);
	cout << "All avaliable platforms: " << endl;

	size_t tmp = 0;
	for (auto i : avaliablePlatforms)
	{
		cout << tmp << ") " << i.getInfo<CL_PLATFORM_NAME>() << endl;
		tmp++;
	}

platformChose:
	cout << "To choose platform enter it's number: " << endl;
	cin >> num;

	if ((0 <= num) && (num < avaliablePlatforms.size()))
	{
		mainPlatform = cl::Platform(avaliablePlatforms[num]);
	}
	else
	{
		cout << "Wrong platform number. Do it again." << endl;
		goto platformChose;
	}
	cout << mainPlatform.getInfo<CL_PLATFORM_NAME>() << " is chosen" << endl;

	mainPlatform.getDevices(CL_DEVICE_TYPE_DEFAULT, &avaliableDevices);
	cout << "All avaliable devices: " << endl;

	tmp = 0;
	for (auto i : avaliableDevices)
	{
		cout << tmp << ") " << i.getInfo<CL_DEVICE_NAME>() << endl;
		tmp++;
	}

deviceChose:
	cout << "To choose device enter it's number: " << endl;
	cin >> num;

	if (0 <= num && num < avaliablePlatforms.size())
	{
		mainDevice = cl::Device(avaliableDevices[num]);
	}
	else
	{
		cout << "Wrong device number. Do it again." << endl;
		goto deviceChose;
	}
	cout << mainDevice.getInfo<CL_DEVICE_NAME>() << " is chosen";

	context = cl::Context(mainDevice);
	queue = cl::CommandQueue(context, mainDevice);

	ifstream file(".\\src\\lib\\clKernel.cl", ios::binary);
	string source;

	istreambuf_iterator<char> inputIt(file), emptyInputIt;
	back_insert_iterator<string> stringInsert(source);
	copy(inputIt, emptyInputIt, stringInsert);

	std::cout << source << std::endl;

	cl::Program::Sources src(1, make_pair(source.c_str(), source.length() + 1));
	cl::Program program(context, src);
	try
	{
		program.build(mainDevice);
	}
	catch (cl::Error error)
	{
		cout << error.what() << '(' << error.err() << ')' << endl;
	}

	try
	{
		clKernel = cl::Kernel(program, "calculation");
	}
	catch (cl::Error error)
	{
		cout << error.what() << '(' << error.err() << ')' << endl;
	}


	try
	{
		Bfractal = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * HEIGHT * WIDTH, fractal);
	}
	catch (cl::Error error)
	{
		cout << error.what() << '(' << error.err() << ')' << endl;
	}
}


void uniqueDevice::Gpu::compute(const real& x, const real& y, const real& scale) {
	using namespace std;

	int iterator = 0;
	clKernel.setArg(iterator++, Bfractal);
	clKernel.setArg(iterator++, x);
	clKernel.setArg(iterator++, y);
	clKernel.setArg(iterator++, scale);
	clKernel.setArg(iterator++, WIDTH);
	clKernel.setArg(iterator++, HEIGHT);
	clKernel.setArg(iterator++, ITERATIONS);

	try
	{
		queue.enqueueNDRangeKernel(clKernel, cl::NullRange, cl::NDRange(WIDTH * HEIGHT), cl::NDRange(50));
	}
	catch (cl::Error error)
	{
		cout << error.what() << '(' << error.err() << ')' << endl;
	}

	try
	{
		queue.finish();
	}
	catch (cl::Error error)
	{
		cout << error.what() << '(' << error.err() << ')' << endl;
	}

	try
	{
		queue.enqueueReadBuffer(Bfractal, CL_TRUE, 0, sizeof(int) * WIDTH * HEIGHT, this->fractal);
	}
	catch (cl::Error error)
	{
		cout << error.what() << '(' << error.err() << ')' << endl;
	}
}

void uniqueDevice::Cpu::mandelbrotSetCreator(int segmentNum) {
	std::mutex mut;
	std::unique_lock lk(mut);

	while (bworking) {
		while (!bstart)
			start.wait(lk);
		real yy = y;
		real xx = x;
		real xx1 = x;
		real yy1 = y;

		int segY = segmentNum / 4;
		int segX = segmentNum % 4;
		xx1 += segX * SEGMENT_SIZE * scale;
		yy -= segY * SEGMENT_SIZE * scale;

		for (int i = 0; i < SEGMENT_SIZE; i++)
		{
			xx = xx1;
			for (int j = 0; j < SEGMENT_SIZE; j++) {
				/// DESCRIBE ABOUT WHAT A HELL IS DOING  IN THIS PART
				real ccx = 0.0;
				real cx1 = 0.0, cy1 = 0.0;
				int counter;
				for (counter = 0; counter < ITERATIONS; counter++)
				{
					ccx = cx1;
					cx1 = cx1 * cx1 - cy1 * cy1 + xx;
					cy1 = cy1 * ccx * 2 + yy;

					if ((cx1 * cx1 + cy1 * cy1) > 4)
					{
						break;
					}
				}
				///
				fractal[(segY * SEGMENT_SIZE + i) * 800 + (segX * SEGMENT_SIZE + j)] = counter;
				xx += scale;
			}
			yy -= scale;
		}
		worksCompleted++;
	}
}

void uniqueDevice::Cpu::init(int* fractal) {
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		threads[i] = std::thread([this, i]() { mandelbrotSetCreator(i); });
	}
	this->fractal = fractal;
}

void uniqueDevice::Cpu::compute(const real& x, const real& y, const real& scale) {
	worksCompleted = 0;
	this->x = x;
	this->y = y;
	this->scale = scale;


	bstart = true;
	start.notify_all();

	while(worksCompleted < NUMBER_OF_THREADS){}
	
	bstart = false;
}

void uniqueDevice::changeDevice() {
	if (curDevice == GPU) curDevice = CPU;
	else curDevice = GPU;
}

void uniqueDevice::setCurrentDevice(Device& obj) {
	curDevice = obj;
}

uniqueDevice::Device uniqueDevice::getCurrentDevice() {
	return curDevice;
}