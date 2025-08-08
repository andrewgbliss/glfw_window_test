message(STATUS "Building glfw_window_test")

cmake_minimum_required(VERSION 3.10)
project(GlfwWindowTest)

# Enable parallel builds
include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif()

# Set build type for faster debug builds
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Optimize for faster compilation
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/external/glfw/include)

# Automatically find all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(glfw_window_test ${SOURCES})

# Enable precompiled headers for faster compilation
target_precompile_headers(glfw_window_test PRIVATE
    <GLFW/glfw3.h>
    <iostream>
    <string>
)

find_package(OpenGL REQUIRED)

if(WIN32)
    message(STATUS "Configuring for Windows")
    target_link_libraries(glfw_window_test PRIVATE ${PROJECT_SOURCE_DIR}/external/glfw/lib-mingw-w64/libglfw3.a OpenGL::GL)
elseif(UNIX)
    message(STATUS "Configuring for Linux")
    find_package(glfw3 REQUIRED)
    target_link_libraries(glfw_window_test PRIVATE glfw OpenGL::GL)
endif()