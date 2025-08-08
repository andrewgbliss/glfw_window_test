#pragma once

#include "Math.h"
#include "GameSettings.h"
#include <GL/gl.h>

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
  void follow(const Position2D &target, float smoothness = 1.0f);
  void centerOn(const Position2D &target) { setPosition(target); }

  // Coordinate conversions
  Position2D screenToWorld(float screenX, float screenY) const;
  Position2D worldToScreen(float worldX, float worldY) const;

  // Camera bounds (what the camera can see in world coordinates)
  float getLeft() const { return position.x - (viewportWidth / zoom) / 2.0f; }
  float getRight() const { return position.x + (viewportWidth / zoom) / 2.0f; }
  float getTop() const { return position.y - (viewportHeight / zoom) / 2.0f; }
  float getBottom() const { return position.y + (viewportHeight / zoom) / 2.0f; }

  // Check if a point or rectangle is visible
  bool isVisible(const Position2D &point) const;
  bool isVisible(const Position2D &topLeft, const Position2D &bottomRight) const;

  // Apply camera transformation to OpenGL
  void apply() const;
  void reset() const;

  // Update camera from game settings
  void updateFromSettings(const GameSettings &settings);

  // Debug information
  void printInfo() const;
};

// Utility functions for common camera behaviors
namespace CameraUtils
{
  // Create a camera that shows pixel-perfect rendering
  Camera createPixelPerfectCamera(int virtualWidth, int virtualHeight);

  // Create a camera that follows a target with boundaries
  Camera createBoundedCamera(int viewportW, int viewportH,
                             float minX, float maxX, float minY, float maxY);
}
