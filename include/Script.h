#pragma once

#include "Math.h"
#include <string>

// Forward declarations
class Node;
class Node2D;
class Node3D;

// Base Script class that can be attached to nodes
class Script
{
protected:
  Node *attachedNode;
  std::string scriptName;

public:
  Script(const std::string &name = "Script") : attachedNode(nullptr), scriptName(name) {}
  virtual ~Script() = default;

  // Called when script is attached to a node
  virtual void onAttach(Node *node) { attachedNode = node; }

  // Called when script is detached from a node
  virtual void onDetach() { attachedNode = nullptr; }

  // Called every frame for updates
  virtual void update(float deltaTime) {}

  // Called every frame for rendering (if needed)
  virtual void render() const {}

  // Getters
  const std::string &getName() const { return scriptName; }
  Node *getAttachedNode() const { return attachedNode; }
  bool isAttached() const { return attachedNode != nullptr; }
};

// Example script: Rotate the attached node
class RotateScript : public Script
{
private:
  float rotationSpeed; // degrees per second

public:
  RotateScript(float speed = 90.0f) : Script("RotateScript"), rotationSpeed(speed) {}

  void update(float deltaTime) override;
  void setRotationSpeed(float speed) { rotationSpeed = speed; }
  float getRotationSpeed() const { return rotationSpeed; }
};

// Example script: Oscillate position
class OscillateScript : public Script
{
private:
  float amplitude;
  float frequency;
  float time;

public:
  OscillateScript(float amp = 0.1f, float freq = 2.0f)
      : Script("OscillateScript"), amplitude(amp), frequency(freq), time(0.0f) {}

  void update(float deltaTime) override;
  void setAmplitude(float amp) { amplitude = amp; }
  void setFrequency(float freq) { frequency = freq; }
  float getAmplitude() const { return amplitude; }
  float getFrequency() const { return frequency; }
};

// Example script: Pulse scale
class PulseScript : public Script
{
private:
  float baseScale;
  float pulseAmount;
  float frequency;
  float time;

public:
  PulseScript(float base = 1.0f, float pulse = 0.2f, float freq = 3.0f)
      : Script("PulseScript"), baseScale(base), pulseAmount(pulse), frequency(freq), time(0.0f) {}

  void update(float deltaTime) override;
  void setBaseScale(float base) { baseScale = base; }
  void setPulseAmount(float pulse) { pulseAmount = pulse; }
  void setFrequency(float freq) { frequency = freq; }
};