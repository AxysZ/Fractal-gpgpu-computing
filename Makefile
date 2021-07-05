CC = clang++
FLAGS = -static -std=c++17 -O3
LIB_FLAGS = -lopencl -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi
LIB_PATH = -L"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.2\lib\x64" -I"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.2\include"
SOURCES = ./src/main.cpp ./src/lib/deviceApi.cpp
EXECUTABLE = multiThreadingFractal
DIR = ./build/

all:
	$(CC) $(FLAGS) $(LIB_FLAGS) $(LIB_PATH) $(SOURCES) -o $(DIR)$(EXECUTABLE).exe