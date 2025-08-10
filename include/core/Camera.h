#pragma once

#include "Math.h"
#include "GameSettings.h"
#include "render/RenderDevice.h"
#include <iostream>
#include <algorithm>
#include <cmath>

class Camera
{
private:
  Position2D position; // Camera position in world coordinates
  float zoom;          // Zoom level (1.0 = normal, 2.0 = 2x zoom in, 0.5 = 2x zoom out)
  int viewportWidth;   // Viewport width in pixels
  int viewportHeight;  // Viewport height in pixels
  bool isDirty;        // Flag to track if projection needs updating

public:
  Camera(int viewportW = 320, int viewportH = 240, const Position2D &pos = Position2D(0.0f, 0.0f), float z = 1.0f)
      : position(pos), zoom(z), viewportWidth(viewportW), viewportHeight(viewportH), isDirty(true) {}

  // Position management
  void setPosition(const Position2D &pos)
  {
    position = pos;
    isDirty = true;
  }
  void setPosition(float x, float y)
  {
    position.x = x;
    position.y = y;
    isDirty = true;
  }
  void move(float deltaX, float deltaY)
  {
    position.x += deltaX;
    position.y += deltaY;
    isDirty = true;
  }
  const Position2D &getPosition() const { return position; }

  // Zoom management
  void setZoom(float z)
  {
    zoom = z > 0.0f ? z : 1.0f;
    isDirty = true;
  }
  void zoomIn(float factor = 1.1f) { setZoom(zoom * factor); }
  void zoomOut(float factor = 1.1f) { setZoom(zoom / factor); }
  float getZoom() const { return zoom; }

  // Viewport management
  void setViewportSize(int width, int height)
  {
    viewportWidth = width;
    viewportHeight = height;
    isDirty = true;
  }
  int getViewportWidth() const { return viewportWidth; }
  int getViewportHeight() const { return viewportHeight; }

  // Camera operations
  void follow(const Position2D &target, float smoothness = 1.0f)
  {
    if (smoothness <= 0.0f || smoothness >= 1.0f)
    {
      // Instant follow
      setPosition(target);
    }
    else
    {
      // Smooth follow using linear interpolation
      float newX = position.x + (target.x - position.x) * smoothness;
      float newY = position.y + (target.y - position.y) * smoothness;
      setPosition(newX, newY);
    }
  }
  void centerOn(const Position2D &target) { setPosition(target); }

  // Coordinate conversions
  Position2D screenToWorld(float screenX, float screenY) const
  {
    // Convert screen coordinates to world coordinates
    float worldX = (screenX / zoom) + getLeft();
    float worldY = (screenY / zoom) + getTop();
    return Position2D(worldX, worldY);
  }

  Position2D worldToScreen(float worldX, float worldY) const
  {
    // Convert world coordinates to screen coordinates
    float screenX = (worldX - getLeft()) * zoom;
    float screenY = (worldY - getTop()) * zoom;
    return Position2D(screenX, screenY);
  }

  // Camera bounds (what the camera can see in world coordinates)
  float getLeft() const { return position.x - (viewportWidth / zoom) / 2.0f; }
  float getRight() const { return position.x + (viewportWidth / zoom) / 2.0f; }
  float getTop() const { return position.y - (viewportHeight / zoom) / 2.0f; }
  float getBottom() const { return position.y + (viewportHeight / zoom) / 2.0f; }

  // Check if a point or rectangle is visible
  bool isVisible(const Position2D &point) const
  {
    return point.x >= getLeft() && point.x <= getRight() &&
           point.y >= getTop() && point.y <= getBottom();
  }

  bool isVisible(const Position2D &topLeft, const Position2D &bottomRight) const
  {
    // Check if rectangle intersects with camera bounds
    return !(bottomRight.x < getLeft() || topLeft.x > getRight() ||
             bottomRight.y < getTop() || topLeft.y > getBottom());
  }

  // Apply camera transformation to OpenGL
  void apply() const
  {
    auto &renderDevice = RenderDevice::getInstance();

    // Calculate the actual view bounds based on camera position and zoom
    float halfWidth = (viewportWidth / zoom) / 2.0f;
    float halfHeight = (viewportHeight / zoom) / 2.0f;

    float left = position.x - halfWidth;
    float right = position.x + halfWidth;
    float top = position.y - halfHeight;
    float bottom = position.y + halfHeight;

    // Set up 2D rendering with camera bounds
    renderDevice.setup2DRendering(viewportWidth, viewportHeight);
  }

  void reset() const
  {
    auto &renderDevice = RenderDevice::getInstance();
    renderDevice.setup2DRendering(viewportWidth, viewportHeight);
  }

  // Update camera from game settings
  void updateFromSettings(const GameSettings &settings)
  {
    setViewportSize(settings.graphics.viewportWidth, settings.graphics.viewportHeight);
  }

  // Debug information
  void printInfo() const
  {
    std::cout << "=== Camera Info ===" << std::endl;
    std::cout << "Position: (" << position.x << ", " << position.y << ")" << std::endl;
    std::cout << "Zoom: " << zoom << std::endl;
    std::cout << "Viewport: " << viewportWidth << "x" << viewportHeight << std::endl;
    std::cout << "View Bounds: Left=" << getLeft() << ", Right=" << getRight()
              << ", Top=" << getTop() << ", Bottom=" << getBottom() << std::endl;
    std::cout << "=================" << std::endl;
  }
};

// Utility functions for common camera behaviors
namespace CameraUtils
{
  // Create a camera that shows pixel-perfect rendering
  Camera createPixelPerfectCamera(int virtualWidth, int virtualHeight)
  {
    return Camera(virtualWidth, virtualHeight, Position2D(virtualWidth / 2.0f, virtualHeight / 2.0f), 1.0f);
  }

  // Create a camera that follows a target with boundaries
  Camera createBoundedCamera(int viewportW, int viewportH,
                             float minX, float maxX, float minY, float maxY)
  {
    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;
    return Camera(viewportW, viewportH, Position2D(centerX, centerY), 1.0f);
  }
}
