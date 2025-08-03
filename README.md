# GLFW Window Test

A simple C++ project that demonstrates how to create a basic OpenGL window using GLFW (Graphics Library Framework). This project serves as a starting point for OpenGL development and window management.

## What it does

This project creates a basic OpenGL window with the following features:

- Creates a 640x480 window titled "Hello World"
- Sets up an OpenGL context
- Runs a basic render loop that clears the screen
- Handles window events and cleanup

## Prerequisites

### Windows

- MinGW-w64 compiler
- CMake (version 3.10 or higher)
- OpenGL development libraries

### Linux

- GCC/G++ compiler
- CMake (version 3.10 or higher)
- OpenGL development libraries
- GLFW3 development libraries

## Building the Project

### Option 1: Using Makefile (Recommended)

The project includes a Makefile that automates the build process for both Windows and Linux:

```bash
# Install dependencies (Linux only)
make install

# Build for both Windows and Linux
make build

# Clean build files
make clean

# Run the Linux version
make run
```

### Option 2: Manual CMake Build

#### Windows

```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw-w64-x86_64.cmake
cmake --build .
```

#### Linux

```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../linux.cmake
cmake --build .
```

## Project Structure

```
glfw_window_test/
├── CMakeLists.txt          # CMake configuration
├── Makefile                # Build automation
├── README.md              # This file
├── external/              # External dependencies
│   └── glfw/             # GLFW library files
│       ├── include/       # GLFW headers
│       └── lib-*/         # Platform-specific libraries
├── src/
│   └── main.cpp          # Main application code
├── include/              # Project headers (currently empty)
├── linux.cmake           # Linux-specific CMake configuration
└── mingw-w64-x86_64.cmake # Windows-specific CMake configuration
```

## Running the Application

After building, you can run the application:

### Windows

```bash
./build/windows/glfw_window_test.exe
```

### Linux

```bash
./build/linux/glfw_window_test
```

## What You'll See

When you run the application, you'll see:

1. A window titled "Hello World" with dimensions 640x480
2. A black screen (cleared OpenGL context)
3. The window will remain open until you close it

## Key Features

- **Cross-platform**: Works on Windows and Linux
- **Simple setup**: Minimal dependencies and straightforward build process
- **OpenGL ready**: Properly configured OpenGL context for further development
- **Event handling**: Basic window event processing

## Next Steps

This project provides a foundation for OpenGL development. You can extend it by:

1. Adding OpenGL rendering code in the main loop
2. Implementing input handling (keyboard, mouse)
3. Adding shaders and 3D rendering
4. Implementing game logic or graphics effects

## Troubleshooting

### Common Issues

**Windows:**

- If you get linker errors, ensure MinGW-w64 is properly installed
- Make sure the GLFW library files are in the correct `external/glfw/lib-*` directory

**Linux:**

- If build fails, run `make install` to install required dependencies
- Ensure you have OpenGL development libraries installed

**General:**

- Make sure CMake version is 3.10 or higher
- Verify that all external GLFW files are present in the `external/glfw` directory

## Dependencies

- **GLFW**: Graphics Library Framework for window management
- **OpenGL**: Graphics API
- **CMake**: Build system generator
- **MinGW-w64** (Windows): C++ compiler
- **GCC/G++** (Linux): C++ compiler

## License

This project uses GLFW which is licensed under the zlib/libpng license. See `external/glfw/LICENSE.md` for details.
