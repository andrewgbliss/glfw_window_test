#pragma once

#include "Node2D.h"
#include "../core/render/RenderDevice.h"

class Rectangle : public Node2D
{
private:
  Color color;
  float width;
  float height;

public:
  Rectangle(const std::string &nodeName = "Rectangle",
            const Position2D &pos = Position2D(),
            const Scale2D &scale = Scale2D(1.0f, 1.0f),
            const Color &col = Colors::green,
            float w = 1.0f, float h = 1.0f)
      : Node2D(nodeName), color(col), width(w), height(h)
  {
    setPosition(pos);
    setScale(scale);
  }

  // Constructors with individual parameters for convenience
  Rectangle(const std::string &nodeName,
            float x, float y,
            float scaleX = 1.0f, float scaleY = 1.0f,
            float r = 0.2f, float g = 0.8f, float b = 0.4f,
            float w = 1.0f, float h = 1.0f)
      : Node2D(nodeName), color(r, g, b), width(w), height(h)
  {
    setPosition(Position2D(x, y));
    setScale(Scale2D(scaleX, scaleY));
  }

  // Color management
  void setColor(const Color &col) { color = col; }
  void setColor(float r, float g, float b) { color = Color(r, g, b); }
  const Color &getColor() const { return color; }

  // Size management
  void setSize(float w, float h)
  {
    width = w;
    height = h;
  }
  float getWidth() const { return width; }
  float getHeight() const { return height; }

  // Render the rectangle
  void render() const override
  {
    auto &renderDevice = RenderDevice::getInstance();

    // Apply transformations
    const Position2D &pos = getTransform().getPosition();
    const Scale2D &scale = getTransform().getScale();
    float rotation = getTransform().getRotation();

    renderDevice.setTransform(pos.x, pos.y, rotation, scale.x, scale.y);

    // Set color
    renderDevice.setColor(color.x, color.y, color.z);

    // Draw rectangle
    renderDevice.drawRectangle(0.0f, 0.0f, width, height);

    renderDevice.resetTransform();
  }
};