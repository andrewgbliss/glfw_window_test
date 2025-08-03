CC = gcc
CXX = g++
MINGW_CXX = x86_64-w64-mingw32-g++

project = glfw_window_test

install:
	sudo apt update
	sudo apt install -y build-essential libgl1-mesa-dev libglfw3-dev
	sudo apt install -y libglew-dev libglm-dev

build: 
	mkdir -p build/windows
	cd build/windows && cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../mingw-w64-x86_64.cmake && cmake --build .

	mkdir -p build/linux
	cd build/linux &&cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../linux.cmake && cmake --build .

	# mkdir -p build/web
	# cd build/web && emcmake cmake ../.. && emmake make

clean:
	rm -rf build

run:
	./build/linux/${project}	



