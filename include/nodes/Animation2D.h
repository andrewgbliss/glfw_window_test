#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

// Animation frame data structure
struct AnimationFrame
{
  int startFrame; // Starting frame index
  int frameCount; // Number of frames in this animation

  AnimationFrame(int start = 0, int count = 1)
      : startFrame(start), frameCount(count) {}
};

// Animation data structure
struct Animation
{
  std::string name;
  std::vector<AnimationFrame> frames;
  float frameRate; // Frames per second
  bool loop;

  Animation(const std::string &animName = "", float fps = 12.0f, bool shouldLoop = true)
      : name(animName), frameRate(fps), loop(shouldLoop) {}
};

class Animation2D
{
private:
  std::map<std::string, Animation> animations;
  std::string currentAnimation;
  float currentFrame;
  float frameTimer;
  bool playing;

public:
  Animation2D();
  ~Animation2D() = default;

  // Animation management
  void addAnimation(const std::string &name, const Animation &animation);
  void addAnimation(const std::string &name, const std::vector<AnimationFrame> &frames,
                    float frameRate = 12.0f, bool loop = true);
  void removeAnimation(const std::string &name);
  bool hasAnimation(const std::string &name) const;

  // Playback control
  void play(const std::string &animationName);
  void stop();
  void pause();
  void resume();
  void reset();

  // Current animation info
  std::string getCurrentAnimation() const { return currentAnimation; }
  bool isPlaying() const { return playing; }
  float getCurrentFrame() const { return currentFrame; }

  // Update animation (call this in your game loop)
  void update(float deltaTime);

  // Get current frame data for rendering
  int getCurrentFrameIndex() const;
  AnimationFrame getCurrentAnimationFrame() const;

  // Animation data access
  const std::map<std::string, Animation> &getAllAnimations() const { return animations; }
  const Animation *getAnimation(const std::string &name) const;

  // Clear all animations
  void clear();
};
