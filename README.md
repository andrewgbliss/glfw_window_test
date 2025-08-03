# GLFW Window Test

A modern C++ project that demonstrates OpenGL window creation using GLFW with a sophisticated Node-based scene graph system. This project showcases modern C++ features including smart pointers, move semantics, and object-oriented design patterns.

## What it does

This project creates an OpenGL window with a comprehensive scene management system:

- **Node-based Scene Graph**: Hierarchical object management with parent-child relationships
- **Modern C++ Features**: Smart pointers, move semantics, RAII, and type safety
- **OpenGL Rendering**: Triangle and rectangle rendering with color management
- **Scene Management**: Multiple predefined scenes with different object configurations
- **Transform System**: Position, scale, and rotation management for all objects

## Modern C++ Features Explained

### Smart Pointers (`std::unique_ptr`)

Smart pointers automatically manage memory, preventing memory leaks and ensuring proper cleanup:

```cpp
// Instead of raw pointers that can leak memory:
Node* node = new Node("MyNode");  // ❌ Manual memory management

// We use smart pointers:
std::unique_ptr<Node> node = std::make_unique<Node>("MyNode");  // ✅ Automatic cleanup
```

**Benefits:**

- **RAII (Resource Acquisition Is Initialization)**: Memory is automatically freed when the pointer goes out of scope
- **Exception Safety**: Memory is cleaned up even if exceptions are thrown
- **No Manual Delete**: Eliminates the need for manual `delete` calls
- **Move Semantics**: Can be moved but not copied, ensuring unique ownership

### `std::make_unique`

A factory function that creates `std::unique_ptr` objects safely:

```cpp
// Creating nodes with make_unique:
auto triangle = std::make_unique<Triangle>("Triangle1",
                                         Position(-0.3f, 0.0f, 0.0f),
                                         Scale(0.5f, 0.5f, 1.0f),
                                         Colors::orange);
```

**Why use `make_unique` instead of `new`?**

- **Exception Safety**: If constructor throws, no memory leak occurs
- **Cleaner Syntax**: No need to specify the type twice
- **Performance**: Can be optimized by the compiler
- **Consistency**: Matches `std::make_shared` pattern

### `std::move` and Move Semantics

Move semantics allow efficient transfer of ownership without copying:

```cpp
// Adding a node to the scene:
auto triangle = std::make_unique<Triangle>("Triangle1", pos, scale, color);
scene.addNode(std::move(triangle));  // Ownership transferred to scene
// triangle is now nullptr and can't be used
```

**What `std::move` does:**

- **Converts to R-value Reference**: Enables move constructor/assignment
- **Transfers Ownership**: Moves resources instead of copying
- **Zero-Cost Abstraction**: No runtime overhead when optimized
- **Prevents Double Deletion**: Ensures unique ownership

### Move Constructor and Assignment

Our classes implement move semantics for efficient resource management:

```cpp
// In Node class:
void addChild(std::unique_ptr<Node> child) {
    if (child) {
        child->setParent(this);
        children.push_back(std::move(child));  // Move into vector
    }
}
```

### RAII (Resource Acquisition Is Initialization)

Objects manage their own resources automatically:

```cpp
class Scene {
private:
    std::unique_ptr<RootNode> rootNode;  // Automatically cleaned up

public:
    Scene(const std::string& sceneName)
        : rootNode(std::make_unique<RootNode>("Root")) {}  // RAII
    // No destructor needed - smart pointer handles cleanup
};
```

### Type Aliases and Modern C++

Using `using` declarations for cleaner code:

```cpp
// Instead of typedef:
typedef Vector3 Position;  // Old C++ style

// Modern C++:
using Position = Vector3;  // Cleaner and more readable
using Scale = Vector3;
using Color = Vector3;
```

### Range-based For Loops

Modern iteration over containers:

```cpp
// Instead of iterator-based loops:
for (auto it = children.begin(); it != children.end(); ++it) {
    (*it)->render();
}

// Modern range-based for:
for (const auto& child : children) {
    child->render();  // Cleaner and less error-prone
}
```

### `auto` Keyword

Automatic type deduction for cleaner code:

```cpp
// Instead of explicit types:
std::unique_ptr<Triangle> triangle = std::make_unique<Triangle>(...);

// Use auto for cleaner code:
auto triangle = std::make_unique<Triangle>(...);  // Type deduced automatically
```

### Const Correctness

Proper use of `const` for safety and optimization:

```cpp
// Const member functions that don't modify state:
const std::string& getName() const { return name; }
const Transform& getTransform() const { return transform; }

// Const references to prevent copying:
void renderRecursive() const {
    for (const auto& child : children) {  // const reference
        child->renderRecursive();
    }
}
```

## Project Structure

```
glfw_window_test/
├── CMakeLists.txt          # CMake configuration
├── Makefile                # Build automation
├── README.md              # This file
├── external/              # External dependencies
│   └── glfw/             # GLFW library files
├── src/
│   └── main.cpp          # Main application with modern C++ features
├── include/              # Modern C++ headers
│   ├── Math.h            # Vector and Transform classes
│   ├── Node.h            # Node-based scene graph system
│   ├── Triangle.h        # Triangle rendering class
│   ├── Rectangle.h       # Rectangle rendering class
│   └── Scene.h           # Scene management with smart pointers
├── linux.cmake           # Linux-specific CMake configuration
└── mingw-w64-x86_64.cmake # Windows-specific CMake configuration
```

## Key Modern C++ Features Used

### 1. Smart Pointers

- **`std::unique_ptr`**: Exclusive ownership, automatic cleanup
- **`std::make_unique`**: Safe smart pointer creation
- **Move semantics**: Efficient ownership transfer

### 2. RAII and Resource Management

- **Automatic cleanup**: No manual memory management
- **Exception safety**: Resources cleaned up even on exceptions
- **Scope-based lifetime**: Objects live exactly as long as needed

### 3. Modern C++ Syntax

- **`auto`**: Automatic type deduction
- **Range-based for loops**: Clean iteration
- **`using` declarations**: Modern type aliases
- **Const correctness**: Safety and optimization

### 4. Object-Oriented Design

- **Virtual functions**: Polymorphic behavior
- **Inheritance hierarchies**: Node → Node2D → Triangle/Rectangle
- **Abstract base classes**: Pure virtual functions for interfaces

### 5. Move Semantics

- **Zero-copy transfers**: Efficient ownership movement
- **Unique ownership**: Prevents double deletion
- **Performance optimization**: Avoids unnecessary copying

## Building the Project

### Option 1: Using Makefile (Recommended)

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

1. **Console Output**: Scene information and node counts
2. **OpenGL Window**: Multiple colored shapes (triangles and rectangles)
3. **Scene Management**: Hierarchical rendering of all objects
4. **Modern C++**: Memory-safe, exception-safe code execution

## Code Examples

### Creating a Scene with Modern C++

```cpp
// Create scene with smart pointers
Scene scene("My Scene");

// Add nodes using make_unique and move semantics
auto triangle = std::make_unique<Triangle>("Triangle1",
                                         Position(0, 0, 0),
                                         Scale(1, 1, 1),
                                         Colors::red);
scene.addNode(std::move(triangle));  // Ownership transferred

// Render the entire scene
scene.render();  // RAII ensures cleanup
```

### Node Hierarchy with Smart Pointers

```cpp
// Parent-child relationships with automatic memory management
auto parent = std::make_unique<Node2D>("Parent");
auto child = std::make_unique<Triangle>("Child");

parent->addChild(std::move(child));  // Child moved to parent
// No manual cleanup needed - smart pointers handle everything
```

## Benefits of Modern C++ Approach

1. **Memory Safety**: No memory leaks or double deletions
2. **Exception Safety**: Resources cleaned up even on exceptions
3. **Performance**: Move semantics avoid unnecessary copying
4. **Readability**: Cleaner, more expressive code
5. **Maintainability**: RAII reduces resource management bugs
6. **Type Safety**: Strong typing prevents many runtime errors

## Next Steps

This project demonstrates modern C++ best practices. You can extend it by:

1. **Adding More Node Types**: Sprites, 3D models, lights
2. **Implementing Animation**: Transform interpolation over time
3. **Adding Physics**: Collision detection and response
4. **Scene Serialization**: Save/load scenes to/from files
5. **Multi-threading**: Parallel rendering and updates

## Troubleshooting

### Common Issues

**Compilation Errors:**

- Ensure C++11 or later is enabled
- Check that all smart pointer includes are present
- Verify move semantics are properly implemented

**Runtime Issues:**

- Smart pointers prevent most memory-related crashes
- RAII ensures proper cleanup even on exceptions
- Const correctness prevents accidental modifications

## Dependencies

- **GLFW**: Graphics Library Framework for window management
- **OpenGL**: Graphics API
- **CMake**: Build system generator
- **C++11 or later**: For smart pointers and move semantics
- **MinGW-w64** (Windows): C++ compiler
- **GCC/G++** (Linux): C++ compiler

## License

This project uses GLFW which is licensed under the zlib/libpng license. See `external/glfw/LICENSE.md` for details.
