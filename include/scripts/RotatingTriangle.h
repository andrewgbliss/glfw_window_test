#pragma once

#include "entities/Triangle.h"
#include <cmath>

// Example of direct inheritance approach
// This creates a specialized Triangle with built-in rotation behavior
class RotatingTriangle : public Triangle
{
private:
  float rotationSpeed; // degrees per second

public:
  RotatingTriangle(const std::string &name, const Position2D &position,
                   const Scale2D &scale, const Color &color, float speed = 90.0f)
      : Triangle(name, position, scale, color), rotationSpeed(speed) {}

  void update(float deltaTime) override
  {
    // Call base class update first
    Triangle::update(deltaTime);

    // Apply rotation
    float currentRotation = getRotation();
    float newRotation = currentRotation + (rotationSpeed * deltaTime);

    // Keep rotation between 0 and 360 degrees
    while (newRotation >= 360.0f)
    {
      newRotation -= 360.0f;
    }

    setRotation(newRotation);
  }

  void setRotationSpeed(float speed) { rotationSpeed = speed; }
  float getRotationSpeed() const { return rotationSpeed; }
};