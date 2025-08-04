#pragma once

#include <GL/gl.h>
#include "core/Node.h"

class Triangle : public Node2D
{
private:
  Color color;

public:
  Triangle(const std::string &nodeName = "Triangle",
           const Position2D &pos = Position2D(),
           const Scale2D &scale = Scale2D(1.0f, 1.0f),
           const Color &col = Colors::orange)
      : Node2D(nodeName), color(col)
  {
    setPosition(pos);
    setScale(scale);
  }

  // Constructors with individual parameters for convenience
  Triangle(const std::string &nodeName,
           float x, float y,
           float scaleX = 1.0f, float scaleY = 1.0f,
           float r = 1.0f, float g = 0.5f, float b = 0.2f)
      : Node2D(nodeName), color(r, g, b)
  {
    setPosition(Position2D(x, y));
    setScale(Scale2D(scaleX, scaleY));
  }

  // Color management
  void setColor(const Color &col) { color = col; }
  void setColor(float r, float g, float b) { color = Color(r, g, b); }
  const Color &getColor() const { return color; }

  // Render the triangle
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

    // Draw triangle
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5f, -0.5f, 0.0f); // Left vertex
    glVertex3f(0.5f, -0.5f, 0.0f);  // Right vertex
    glVertex3f(0.0f, 0.5f, 0.0f);   // Top vertex
    glEnd();

    glPopMatrix();
  }
};