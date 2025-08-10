#pragma once

#include "RenderDriver.h"
#include <memory>
#include <string>
#include <iostream>

// Forward declarations
class Window;

// Singleton RenderDevice class that manages the render driver
class RenderDevice
{
private:
  static RenderDevice *s_instance;
  std::unique_ptr<RenderDriver> m_driver;
  bool m_initialized;

  // Private constructor for singleton pattern
  RenderDevice() : m_initialized(false) {}

public:
  // Singleton access
  static RenderDevice &getInstance()
  {
    if (!s_instance)
    {
      s_instance = new RenderDevice();
    }
    return *s_instance;
  }

  // Prevent copying and assignment
  RenderDevice(const RenderDevice &) = delete;
  RenderDevice &operator=(const RenderDevice &) = delete;

  // Driver management
  void setDriver(std::unique_ptr<RenderDriver> driver)
  {
    if (m_initialized)
    {
      std::cerr << "RenderDevice: Cannot change driver after initialization" << std::endl;
      return;
    }

    m_driver = std::move(driver);
    std::cout << "RenderDevice: Driver set to " << (m_driver ? m_driver->getDriverName() : "null") << std::endl;
  }

  RenderDriver *getDriver() const
  {
    return m_driver.get();
  }

  // Initialization
  bool initialize(Window *window)
  {
    if (m_initialized)
    {
      return true;
    }

    if (!m_driver)
    {
      std::cerr << "RenderDevice: No driver set" << std::endl;
      return false;
    }

    if (!m_driver->initialize(window))
    {
      std::cerr << "RenderDevice: Failed to initialize driver" << std::endl;
      return false;
    }

    m_initialized = true;
    std::cout << "RenderDevice: Initialized with " << m_driver->getDriverName() << " " << m_driver->getVersion() << std::endl;
    return true;
  }

  void cleanup()
  {
    if (m_driver)
    {
      m_driver->cleanup();
    }
    m_initialized = false;
    std::cout << "RenderDevice: Cleaned up" << std::endl;
  }

  bool isInitialized() const { return m_initialized; }

  // Driver information
  std::string getDriverName() const
  {
    return m_driver ? m_driver->getDriverName() : "No Driver";
  }

  std::string getVersion() const
  {
    return m_driver ? m_driver->getVersion() : "Unknown";
  }

  // Convenience methods that delegate to the driver
  void setup2DRendering(int viewportWidth, int viewportHeight)
  {
    if (m_driver)
      m_driver->setup2DRendering(viewportWidth, viewportHeight);
  }

  void clear(float r, float g, float b, float a)
  {
    if (m_driver)
      m_driver->clear(r, g, b, a);
  }

  void setTransform(float x, float y, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f)
  {
    if (m_driver)
      m_driver->setTransform(x, y, rotation, scaleX, scaleY);
  }

  void resetTransform()
  {
    if (m_driver)
      m_driver->resetTransform();
  }

  void setColor(float r, float g, float b, float a = 1.0f)
  {
    if (m_driver)
      m_driver->setColor(r, g, b, a);
  }

  void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
  {
    if (m_driver)
      m_driver->drawTriangle(x1, y1, x2, y2, x3, y3);
  }

  void drawRectangle(float x, float y, float width, float height)
  {
    if (m_driver)
      m_driver->drawRectangle(x, y, width, height);
  }

  void drawSprite(float x, float y, float width, float height, unsigned int textureId, float texLeft = 0.0f, float texTop = 0.0f, float texRight = 1.0f, float texBottom = 1.0f)
  {
    if (m_driver)
      m_driver->drawSprite(x, y, width, height, textureId, texLeft, texTop, texRight, texBottom);
  }

  unsigned int createTexture()
  {
    return m_driver ? m_driver->createTexture() : 0;
  }

  void deleteTexture(unsigned int textureId)
  {
    if (m_driver)
      m_driver->deleteTexture(textureId);
  }

  void uploadTexture(unsigned int textureId, int width, int height, const void *data, bool useLinearFiltering = true)
  {
    if (m_driver)
      m_driver->uploadTexture(textureId, width, height, data, useLinearFiltering);
  }

  // Window management (rendering-related)
  void swapBuffers()
  {
    if (m_driver)
      m_driver->swapBuffers();
  }

  void pollEvents()
  {
    if (m_driver)
      m_driver->pollEvents();
  }

  bool shouldClose() const
  {
    return m_driver ? m_driver->shouldClose() : true;
  }
};

// Static member definition
RenderDevice *RenderDevice::s_instance = nullptr;
