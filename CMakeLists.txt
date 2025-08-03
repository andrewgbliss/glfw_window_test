message(STATUS "Building glfw_window_test")

cmake_minimum_required(VERSION 3.10)
project(GlfwWindowTest)

include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/external/glfw/include)

add_executable(glfw_window_test src/main.cpp)

find_package(OpenGL REQUIRED)

if(WIN32)
    message(STATUS "Configuring for Windows")
    target_link_libraries(glfw_window_test PRIVATE ${PROJECT_SOURCE_DIR}/external/glfw/lib-mingw-w64/libglfw3.a OpenGL::GL)
elseif(UNIX)
    message(STATUS "Configuring for Linux")
    find_package(glfw3 REQUIRED)
    target_link_libraries(glfw_window_test PRIVATE glfw OpenGL::GL)
endif()