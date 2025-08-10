#pragma once

#include <iostream>
#include <string>
#include "../GameSettings.h"

// Window configuration structure
struct WindowConfig
{
  int width = 800;
  int height = 600;
  std::string title = "Window";
  bool resizable = true;
  bool visible = true;
  bool decorated = true;
  bool focused = true;
  bool maximized = false;
  bool centerOnScreen = true;
};

// Abstract base class for window management
class Window
{
protected:
  bool m_initialized;

public:
  Window() : m_initialized(false) {}
  virtual ~Window() = default;

  // Check if window is valid
  virtual bool isValid() const = 0;

  // Check if window should close
  virtual bool shouldClose() const = 0;

  // Poll events
  virtual void pollEvents() = 0;

  // Swap buffers
  virtual void swapBuffers() = 0;

  // Set window title
  virtual void setTitle(const std::string &title) = 0;

  // Get window size
  virtual void getSize(int &width, int &height) const = 0;

  // Set window size
  virtual void setSize(int width, int height) = 0;

  // Show/hide window
  virtual void show() = 0;
  virtual void hide() = 0;

  // Check if a key is pressed
  virtual bool isKeyPressed(int key) const = 0;

  // Get native window handle (for render driver initialization)
  virtual void *getNativeWindow() const = 0;
};

// Factory function for creating windows
Window *createWindow(const WindowConfig &config = WindowConfig(), const GameSettings *settings = nullptr);