#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

// Window configuration structure
struct WindowConfig
{
  int width = 800;
  int height = 600;
  std::string title = "GLFW Window";
  bool resizable = true;
  bool visible = true;
  bool decorated = true;
  bool focused = true;
  bool maximized = false;
  bool centerOnScreen = true;
};

// Error callback function
void errorCallback(int error, const char *description)
{
  std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Window creation function with better API
GLFWwindow *createWindow(const WindowConfig &config = WindowConfig())
{
  // Set error callback
  glfwSetErrorCallback(errorCallback);

  // Initialize GLFW
  if (!glfwInit())
  {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return nullptr;
  }

  // Configure window hints
  glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_VISIBLE, config.visible ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_DECORATED, config.decorated ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_FOCUSED, config.focused ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_MAXIMIZED, config.maximized ? GLFW_TRUE : GLFW_FALSE);

  // Create the window
  GLFWwindow *window = glfwCreateWindow(
      config.width,
      config.height,
      config.title.c_str(),
      NULL,
      NULL);

  if (!window)
  {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }

  // Center window on screen if requested
  if (config.centerOnScreen)
  {
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (monitor)
    {
      const GLFWvidmode *mode = glfwGetVideoMode(monitor);
      int x = (mode->width - config.width) / 2;
      int y = (mode->height - config.height) / 2;
      glfwSetWindowPos(window, x, y);
    }
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Set vsync
  glfwSwapInterval(1);

  return window;
}

// Window management class for better encapsulation
class Window
{
private:
  GLFWwindow *m_window;
  bool m_initialized;

public:
  Window(const WindowConfig &config = WindowConfig()) : m_window(nullptr), m_initialized(false)
  {
    m_window = createWindow(config);
    m_initialized = (m_window != nullptr);
  }

  ~Window()
  {
    if (m_initialized)
    {
      glfwTerminate();
    }
  }

  // Check if window is valid
  bool isValid() const
  {
    return m_initialized && m_window != nullptr;
  }

  // Check if window should close
  bool shouldClose() const
  {
    return glfwWindowShouldClose(m_window);
  }

  // Poll events
  void pollEvents()
  {
    glfwPollEvents();
  }

  // Swap buffers
  void swapBuffers()
  {
    glfwSwapBuffers(m_window);
  }

  // Get the underlying GLFW window
  GLFWwindow *getGLFWWindow() const
  {
    return m_window;
  }

  // Set window title
  void setTitle(const std::string &title)
  {
    glfwSetWindowTitle(m_window, title.c_str());
  }

  // Get window size
  void getSize(int &width, int &height) const
  {
    glfwGetWindowSize(m_window, &width, &height);
  }

  // Set window size
  void setSize(int width, int height)
  {
    glfwSetWindowSize(m_window, width, height);
  }

  // Show/hide window
  void show()
  {
    glfwShowWindow(m_window);
  }

  void hide()
  {
    glfwHideWindow(m_window);
  }
};