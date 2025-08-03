#pragma once

#include "Script.h"
#include "Node.h"
#include <cmath>

// Implementation of RotateScript
inline void RotateScript::update(float deltaTime)
{
  if (!attachedNode)
    return;

  // Only work with Node2D and Node3D that have rotation
  if (auto node2D = dynamic_cast<Node2D *>(attachedNode))
  {
    float currentRotation = node2D->getRotation();
    float newRotation = currentRotation + (rotationSpeed * deltaTime);

    // Keep rotation between 0 and 360 degrees
    while (newRotation >= 360.0f)
    {
      newRotation -= 360.0f;
    }

    node2D->setRotation(newRotation);
  }
  else if (auto node3D = dynamic_cast<Node3D *>(attachedNode))
  {
    Vector3 currentRotation = node3D->getRotation();
    Vector3 newRotation = currentRotation + Vector3(0, 0, rotationSpeed * deltaTime);
    node3D->setRotation(newRotation);
  }
}

// Implementation of OscillateScript
inline void OscillateScript::update(float deltaTime)
{
  if (!attachedNode)
    return;

  time += deltaTime;

  if (auto node2D = dynamic_cast<Node2D *>(attachedNode))
  {
    Position2D originalPos = node2D->getPosition();
    float offset = amplitude * sin(frequency * time);
    node2D->setPosition(Position2D(originalPos.x + offset, originalPos.y));
  }
}

// Implementation of PulseScript
inline void PulseScript::update(float deltaTime)
{
  if (!attachedNode)
    return;

  time += deltaTime;

  if (auto node2D = dynamic_cast<Node2D *>(attachedNode))
  {
    float scaleFactor = baseScale + pulseAmount * sin(frequency * time);
    node2D->setScale(Scale2D(scaleFactor, scaleFactor));
  }
}