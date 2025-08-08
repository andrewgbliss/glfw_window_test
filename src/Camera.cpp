#include "../include/core/Camera.h"
#include <iostream>
#include <algorithm>
#include <cmath>

void Camera::follow(const Position2D &target, float smoothness)
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

Position2D Camera::screenToWorld(float screenX, float screenY) const
{
  // Convert screen coordinates to world coordinates
  float worldX = (screenX / zoom) + getLeft();
  float worldY = (screenY / zoom) + getTop();
  return Position2D(worldX, worldY);
}

Position2D Camera::worldToScreen(float worldX, float worldY) const
{
  // Convert world coordinates to screen coordinates
  float screenX = (worldX - getLeft()) * zoom;
  float screenY = (worldY - getTop()) * zoom;
  return Position2D(screenX, screenY);
}

bool Camera::isVisible(const Position2D &point) const
{
  return point.x >= getLeft() && point.x <= getRight() &&
         point.y >= getTop() && point.y <= getBottom();
}

bool Camera::isVisible(const Position2D &topLeft, const Position2D &bottomRight) const
{
  // Check if rectangle intersects with camera bounds
  return !(bottomRight.x < getLeft() || topLeft.x > getRight() ||
           bottomRight.y < getTop() || topLeft.y > getBottom());
}

void Camera::apply() const
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Calculate the actual view bounds based on camera position and zoom
  float halfWidth = (viewportWidth / zoom) / 2.0f;
  float halfHeight = (viewportHeight / zoom) / 2.0f;

  float left = position.x - halfWidth;
  float right = position.x + halfWidth;
  float top = position.y - halfHeight;
  float bottom = position.y + halfHeight;

  glOrtho(left, right, bottom, top, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Camera::reset() const
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, viewportWidth, viewportHeight, 0.0f, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Camera::updateFromSettings(const GameSettings &settings)
{
  setViewportSize(settings.graphics.viewportWidth, settings.graphics.viewportHeight);
}

void Camera::printInfo() const
{
  std::cout << "=== Camera Info ===" << std::endl;
  std::cout << "Position: (" << position.x << ", " << position.y << ")" << std::endl;
  std::cout << "Zoom: " << zoom << std::endl;
  std::cout << "Viewport: " << viewportWidth << "x" << viewportHeight << std::endl;
  std::cout << "View Bounds: Left=" << getLeft() << ", Right=" << getRight()
            << ", Top=" << getTop() << ", Bottom=" << getBottom() << std::endl;
  std::cout << "=================" << std::endl;
}

namespace CameraUtils
{
  Camera createPixelPerfectCamera(int virtualWidth, int virtualHeight)
  {
    return Camera(virtualWidth, virtualHeight, Position2D(virtualWidth / 2.0f, virtualHeight / 2.0f), 1.0f);
  }

  Camera createBoundedCamera(int viewportW, int viewportH,
                             float minX, float maxX, float minY, float maxY)
  {
    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;
    return Camera(viewportW, viewportH, Position2D(centerX, centerY), 1.0f);
  }
}
