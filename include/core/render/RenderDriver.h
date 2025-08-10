#pragma once

#include <string>

// Forward declarations
class Window;

// Abstract base class for render drivers
class RenderDriver
{
public:
  virtual ~RenderDriver() = default;

  // Initialization and cleanup
  virtual bool initialize(Window *window) = 0;
  virtual void cleanup() = 0;

  // Driver information
  virtual std::string getDriverName() const = 0;
  virtual std::string getVersion() const = 0;

  // High-level rendering methods
  virtual void setup2DRendering(int viewportWidth, int viewportHeight) = 0;
  virtual void clear(float r, float g, float b, float a) = 0;
  virtual void setTransform(float x, float y, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f) = 0;
  virtual void resetTransform() = 0;
  virtual void setColor(float r, float g, float b, float a = 1.0f) = 0;
  virtual void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) = 0;
  virtual void drawRectangle(float x, float y, float width, float height) = 0;
  virtual void drawSprite(float x, float y, float width, float height, unsigned int textureId, float texLeft = 0.0f, float texTop = 0.0f, float texRight = 1.0f, float texBottom = 1.0f) = 0;

  // Texture management
  virtual unsigned int createTexture() = 0;
  virtual void deleteTexture(unsigned int textureId) = 0;
  virtual void uploadTexture(unsigned int textureId, int width, int height, const void *data, bool useLinearFiltering = true) = 0;

  // Window management (rendering-related)
  virtual void swapBuffers() = 0;
  virtual void pollEvents() = 0;
  virtual bool shouldClose() const = 0;
};
