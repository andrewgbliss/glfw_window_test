#pragma once

#include <GL/gl.h>
#include "core/Node.h"

class Rectangle : public Node2D
{
private:
  Color color;

public:
  Rectangle(const std::string &nodeName = "Rectangle",
            const Position2D &pos = Position2D(),
            const Scale2D &scale = Scale2D(1.0f, 1.0f),
            const Color &col = Colors::green)
      : Node2D(nodeName), color(col)
  {
    setPosition(pos);
    setScale(scale);
  }

  // Constructors with individual parameters for convenience
  Rectangle(const std::string &nodeName,
            float x, float y,
            float scaleX = 1.0f, float scaleY = 1.0f,
            float r = 0.2f, float g = 0.8f, float b = 0.4f)
      : Node2D(nodeName), color(r, g, b)
  {
    setPosition(Position2D(x, y));
    setScale(Scale2D(scaleX, scaleY));
  }

  // Color management
  void setColor(const Color &col) { color = col; }
  void setColor(float r, float g, float b) { color = Color(r, g, b); }
  const Color &getColor() const { return color; }

  // Render the rectangle
  void render() const override
  {
    glPushMatrix();

    // Apply transformations
    const Position2D &pos = getTransform().getPosition();
    const Scale2D &scale = getTransform().getScale();
    float rotation = getTransform().getRotation();

    glTranslatef(pos.x, pos.y, 0.0f);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glScalef(scale.x, scale.y, 1.0f);

    // Set color
    glColor3f(color.x, color.y, color.z);

    // Draw rectangle (two triangles)
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, 0.0f); // Bottom-left
    glVertex3f(0.5f, -0.5f, 0.0f);  // Bottom-right
    glVertex3f(0.5f, 0.5f, 0.0f);   // Top-right
    glVertex3f(-0.5f, 0.5f, 0.0f);  // Top-left
    glEnd();

    glPopMatrix();
  }
};