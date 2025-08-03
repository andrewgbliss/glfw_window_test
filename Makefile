CC = gcc
CXX = g++
MINGW_CXX = x86_64-w64-mingw32-g++

project = glfw_window_test

install:
	sudo apt update
	sudo apt install -y build-essential libgl1-mesa-dev libglfw3-dev
	sudo apt install -y libglew-dev libglm-dev

# Hot reload - watches for changes and rebuilds automatically
watch:
	@echo "🔥 Hot reload mode - watching for changes..."
	@echo "Press Ctrl+C to stop"
	@echo "Watching: src/, include/, CMakeLists.txt"
	@while true; do \
		echo "⏳ Waiting for changes..."; \
		sleep 2; \
		echo "🔄 Checking for changes..."; \
		make go; \
		echo "✅ Hot reload cycle complete!"; \
	done

# Build and run in one command - fastest development workflow
go:
	@echo "🚀 Building and running..."
	@mkdir -p build/linux
	@cd build/linux && cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../linux.cmake && cmake --build . --parallel --target glfw_window_test
	@echo "✅ Build complete! Running application..."
	@./build/linux/${project}

# Fast development build - only rebuilds what changed
dev:
	@echo "🔨 Fast development build..."
	@mkdir -p build/linux
	@cd build/linux && cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../linux.cmake && cmake --build . --parallel --target glfw_window_test
	@echo "✅ Build complete!"

# Quick rebuild - fastest option
quick:
	@echo "⚡ Quick rebuild..."
	@cd build/linux && cmake --build . --parallel --target glfw_window_test
	@echo "✅ Rebuild complete!"

# Build only Linux (faster for development)
build-linux:
	@echo "🐧 Building Linux version..."
	@mkdir -p build/linux
	@cd build/linux && cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../linux.cmake && cmake --build . --parallel

# Build only Windows
build-windows:
	@echo "🪟 Building Windows version..."
	@mkdir -p build/windows
	@cd build/windows && cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../mingw-w64-x86_64.cmake && cmake --build . --parallel

# Fast build - only rebuild what changed
build-fast: 
	@echo "🚀 Fast build with parallel compilation..."
	@mkdir -p build/windows
	@cd build/windows && cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../mingw-w64-x86_64.cmake && cmake --build . --parallel
	@mkdir -p build/linux
	@cd build/linux && cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../linux.cmake && cmake --build . --parallel

# Full build (original)
build: 
	@echo "🔨 Full build..."
	@mkdir -p build/windows
	@cd build/windows && cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../mingw-w64-x86_64.cmake && cmake --build .
	@mkdir -p build/linux
	@cd build/linux && cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../linux.cmake && cmake --build .

clean:
	@echo "🧹 Cleaning build directories..."
	@rm -rf build

run:
	@echo "🚀 Running application..."
	@./build/linux/${project}

# Help target
help:
	@echo "Available targets:"
	@echo "  watch    - Hot reload mode (watches for changes)"
	@echo "  go       - Build quickly and run (one command)"
	@echo "  dev      - Fast development build (Linux only, incremental)"
	@echo "  quick    - Quick rebuild (fastest, requires previous build)"
	@echo "  build-linux  - Build Linux version only"
	@echo "  build-windows - Build Windows version only"
	@echo "  build-fast - Fast build with parallel compilation"
	@echo "  build    - Full build (original)"
	@echo "  clean    - Clean all build directories"
	@echo "  run      - Run the application"
	@echo "  install  - Install dependencies"
	@echo "  help     - Show this help message"	



