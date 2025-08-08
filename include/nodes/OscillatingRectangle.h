#pragma once

#include "Rectangle.h"
#include <cmath>

// Example of direct inheritance approach
// This creates a specialized Rectangle with built-in oscillation behavior
class OscillatingRectangle : public Rectangle
{
private:
  float amplitude;
  float frequency;
  float time;
  Position2D originalPosition;

public:
  OscillatingRectangle(const std::string &name, const Position2D &position,
                       const Scale2D &scale, const Color &color,
                       float amp = 0.1f, float freq = 2.0f)
      : Rectangle(name, position, scale, color),
        amplitude(amp), frequency(freq), time(0.0f), originalPosition(position) {}

  void update(float deltaTime) override
  {
    // Call base class update first
    Rectangle::update(deltaTime);

    // Apply oscillation
    time += deltaTime;
    float offset = amplitude * sin(frequency * time);
    setPosition(Position2D(originalPosition.x + offset, originalPosition.y));
  }

  void setAmplitude(float amp) { amplitude = amp; }
  void setFrequency(float freq) { frequency = freq; }
  float getAmplitude() const { return amplitude; }
  float getFrequency() const { return frequency; }
};