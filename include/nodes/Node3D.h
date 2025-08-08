#pragma once

#include "../core/Math.h"
#include "Node.h"
#include <vector>
#include <memory>
#include <string>

// 3D Node class - has 3D position, scale, and rotation
class Node3D : public Node
{
protected:
  Transform3D transform;

public:
  Node3D(const std::string &nodeName = "Node3D") : Node(nodeName) {}

  // Transform management
  Transform3D &getTransform() { return transform; }
  const Transform3D &getTransform() const { return transform; }

  void setPosition(const Position &pos) { transform.setPosition(pos); }
  void setScale(const Scale &scale) { transform.setScale(scale); }
  void setRotation(const Vector3 &rotation) { transform.setRotation(rotation); }

  const Position &getPosition() const { return transform.getPosition(); }
  const Scale &getScale() const { return transform.getScale(); }
  const Vector3 &getRotation() const { return transform.getRotation(); }

  void render() const override = 0;
  void update(float deltaTime = 0.0f) override;
};

inline void Node3D::update(float deltaTime)
{
  // Call base class update
  Node::update(deltaTime);
}