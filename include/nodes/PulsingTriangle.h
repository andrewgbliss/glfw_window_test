#pragma once

#include "Triangle.h"
#include <cmath>

// Example of direct inheritance approach
// This creates a specialized Triangle with built-in pulsing behavior
class PulsingTriangle : public Triangle
{
private:
  float baseScale;
  float pulseAmount;
  float frequency;
  float time;

public:
  PulsingTriangle(const std::string &name, const Position2D &position,
                  const Scale2D &scale, const Color &color,
                  float base = 1.0f, float pulse = 0.2f, float freq = 3.0f)
      : Triangle(name, position, scale, color),
        baseScale(base), pulseAmount(pulse), frequency(freq), time(0.0f) {}

  void update(float deltaTime) override
  {
    // Call base class update first
    Triangle::update(deltaTime);

    // Apply pulsing
    time += deltaTime;
    float scaleFactor = baseScale + pulseAmount * sin(frequency * time);
    setScale(Scale2D(scaleFactor, scaleFactor));
  }

  void setBaseScale(float base) { baseScale = base; }
  void setPulseAmount(float pulse) { pulseAmount = pulse; }
  void setFrequency(float freq) { frequency = freq; }
};