#pragma once

#include "../core/Math.h"
#include "Node.h"
#include <vector>
#include <memory>
#include <string>

// 2D Node class - has 2D position, scale, and rotation
class Node2D : public Node
{
protected:
  Transform2D transform;

public:
  Node2D(const std::string &nodeName = "Node2D") : Node(nodeName) {}

  // Transform management
  Transform2D &getTransform() { return transform; }
  const Transform2D &getTransform() const { return transform; }

  void setPosition(const Position2D &pos) { transform.setPosition(pos); }
  void setScale(const Scale2D &scale) { transform.setScale(scale); }
  void setRotation(float rotation) { transform.setRotation(rotation); }

  const Position2D &getPosition() const { return transform.getPosition(); }
  const Scale2D &getScale() const { return transform.getScale(); }
  float getRotation() const { return transform.getRotation(); }

  void render() const override = 0;
  void update(float deltaTime = 0.0f) override;
};

inline void Node2D::update(float deltaTime)
{
  // Call base class update
  Node::update(deltaTime);
}
