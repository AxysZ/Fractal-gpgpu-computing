# Fractal explorer with supporting gpgpu and threadpool

This program calculating Mandelbrot set using multithreading or gpu computations. For detail about fractals see: [wikipedia page](https://en.wikipedia.org/wiki/Mandelbrot_set).

### Aditional libs:

* [OpenCL](https://en.wikipedia.org/wiki/OpenCL) (for using gpu for calculations)
* [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine) (for rendering screen)

### How to use this program?

* Key W for going up in complex plane
* Key A for going left in complex plane
* Key S for going dowm in complex plane
* Key D for going right in complex plane

* Keys arrow up and arrow down for zoon in and zoom out respectively
* And finaly key C to change mode (between cpu and gpu)


### How to buind this program?

If you have make on your system just type:

``` 
make
```

If you dont have make, add dir build and type:

```
"CC" -static -std=c++17 -O3 -lopencl -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -L"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.2\lib\x64" -I"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.2\include" t\CUDA\v11.2\include" ./src/main.cpp ./src/lib/deviceApi.cpp -o ./build/multiThreadingFractal.exe
```

command, where "CC" is c++ compiler that you use, for example: clang++.