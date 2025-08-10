#pragma once

#include "Window.h"
#include <GLFW/glfw3.h>
#include <iostream>

// OpenGL-specific window implementation
class OpenGLWindow : public Window
{
private:
  GLFWwindow *m_window;

  // Error callback function
  static void errorCallback(int error, const char *description)
  {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
  }

public:
  OpenGLWindow(const WindowConfig &config = WindowConfig(), const GameSettings *settings = nullptr) : m_window(nullptr)
  {
    // Set error callback
    glfwSetErrorCallback(errorCallback);

    // Initialize GLFW
    if (!glfwInit())
    {
      std::cerr << "Failed to initialize GLFW" << std::endl;
      return;
    }

    // Configure window hints
    glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, config.visible ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, config.decorated ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUSED, config.focused ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED, config.maximized ? GLFW_TRUE : GLFW_FALSE);

    // Create the window
    m_window = glfwCreateWindow(
        config.width,
        config.height,
        config.title.c_str(),
        NULL,
        NULL);

    if (!m_window)
    {
      std::cerr << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return;
    }

    // Center window on screen if requested
    if (config.centerOnScreen)
    {
      GLFWmonitor *monitor = glfwGetPrimaryMonitor();
      if (monitor)
      {
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        int monitorX, monitorY;
        glfwGetMonitorPos(monitor, &monitorX, &monitorY);
        int windowX = monitorX + (mode->width - config.width) / 2;
        int windowY = monitorY + (mode->height - config.height) / 2;
        glfwSetWindowPos(m_window, windowX, windowY);
      }
    }

    m_initialized = true;
  }

  ~OpenGLWindow()
  {
    if (m_initialized)
    {
      glfwTerminate();
    }
  }

  // Implementation of Window interface
  bool isValid() const override
  {
    return m_initialized && m_window != nullptr;
  }

  bool shouldClose() const override
  {
    return glfwWindowShouldClose(m_window);
  }

  void pollEvents() override
  {
    glfwPollEvents();
  }

  void swapBuffers() override
  {
    glfwSwapBuffers(m_window);
  }

  void setTitle(const std::string &title) override
  {
    glfwSetWindowTitle(m_window, title.c_str());
  }

  void getSize(int &width, int &height) const override
  {
    glfwGetWindowSize(m_window, &width, &height);
  }

  void setSize(int width, int height) override
  {
    glfwSetWindowSize(m_window, width, height);
  }

  void show() override
  {
    glfwShowWindow(m_window);
  }

  void hide() override
  {
    glfwHideWindow(m_window);
  }

  bool isKeyPressed(int key) const override
  {
    return glfwGetKey(m_window, key) == GLFW_PRESS;
  }

  void *getNativeWindow() const override
  {
    return static_cast<void *>(m_window);
  }

  // OpenGL-specific methods
  GLFWwindow *getGLFWWindow() const { return m_window; }
};
