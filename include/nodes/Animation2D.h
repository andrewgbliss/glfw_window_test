#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <iostream>

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
  Animation2D()
      : currentAnimation(""), currentFrame(0.0f), frameTimer(0.0f), playing(false)
  {
  }
  ~Animation2D() = default;

  // Animation management
  void addAnimation(const std::string &name, const Animation &animation)
  {
    animations[name] = animation;
    std::cout << "Added animation: " << name << " with " << animation.frames.size() << " frame groups" << std::endl;
  }

  void addAnimation(const std::string &name, const std::vector<AnimationFrame> &frames,
                    float frameRate = 12.0f, bool loop = true)
  {
    Animation anim(name, frameRate, loop);
    anim.frames = frames;
    animations[name] = anim;
    std::cout << "Added animation: " << name << " with " << frames.size() << " frame groups" << std::endl;
  }

  void removeAnimation(const std::string &name)
  {
    auto it = animations.find(name);
    if (it != animations.end())
    {
      animations.erase(it);
      if (currentAnimation == name)
      {
        currentAnimation = "";
        currentFrame = 0.0f;
        playing = false;
      }
    }
  }

  bool hasAnimation(const std::string &name) const
  {
    return animations.find(name) != animations.end();
  }

  // Playback control
  void play(const std::string &animationName)
  {
    if (animations.find(animationName) == animations.end())
    {
      std::cerr << "Animation not found: " << animationName << std::endl;
      return;
    }

    currentAnimation = animationName;
    currentFrame = 0.0f;
    frameTimer = 0.0f;
    playing = true;
    std::cout << "Playing animation: " << animationName << std::endl;
  }

  void stop()
  {
    playing = false;
    currentFrame = 0.0f;
    frameTimer = 0.0f;
  }

  void pause()
  {
    playing = false;
  }

  void resume()
  {
    if (!currentAnimation.empty())
    {
      playing = true;
    }
  }

  void reset()
  {
    currentFrame = 0.0f;
    frameTimer = 0.0f;
  }

  // Current animation info
  std::string getCurrentAnimation() const { return currentAnimation; }
  bool isPlaying() const { return playing; }
  float getCurrentFrame() const { return currentFrame; }

  // Update animation (call this in your game loop)
  void update(float deltaTime)
  {
    if (!playing || currentAnimation.empty())
      return;

    auto it = animations.find(currentAnimation);
    if (it == animations.end())
      return;

    const Animation &anim = it->second;
    if (anim.frames.empty())
      return;

    // Update frame timer
    frameTimer += deltaTime;
    float frameTime = 1.0f / anim.frameRate;

    if (frameTimer >= frameTime)
    {
      frameTimer -= frameTime;
      currentFrame += 1.0f;

      // Calculate total frames in current animation
      int totalFrames = 0;
      for (const auto &frame : anim.frames)
      {
        totalFrames += frame.frameCount;
      }

      // Handle looping or stopping
      if (currentFrame >= totalFrames)
      {
        if (anim.loop)
        {
          currentFrame = 0.0f;
        }
        else
        {
          currentFrame = static_cast<float>(totalFrames - 1);
          playing = false;
        }
      }
    }
  }

  // Get current frame data for rendering
  int getCurrentFrameIndex() const
  {
    if (currentAnimation.empty())
      return 0;

    auto it = animations.find(currentAnimation);
    if (it == animations.end())
      return 0;

    const Animation &anim = it->second;
    if (anim.frames.empty())
      return 0;

    int frameIndex = static_cast<int>(currentFrame);
    int currentFrameInAnimation = 0;

    for (const auto &frame : anim.frames)
    {
      if (frameIndex < frame.frameCount)
      {
        return frame.startFrame + frameIndex;
      }
      frameIndex -= frame.frameCount;
      currentFrameInAnimation += frame.frameCount;
    }

    // Fallback to first frame of first animation
    return anim.frames[0].startFrame;
  }

  AnimationFrame getCurrentAnimationFrame() const
  {
    if (currentAnimation.empty())
      return AnimationFrame(0, 1);

    auto it = animations.find(currentAnimation);
    if (it == animations.end())
      return AnimationFrame(0, 1);

    const Animation &anim = it->second;
    if (anim.frames.empty())
      return AnimationFrame(0, 1);

    int frameIndex = static_cast<int>(currentFrame);

    for (const auto &frame : anim.frames)
    {
      if (frameIndex < frame.frameCount)
      {
        return frame;
      }
      frameIndex -= frame.frameCount;
    }

    // Fallback to first frame
    return anim.frames[0];
  }

  // Animation data access
  const std::map<std::string, Animation> &getAllAnimations() const { return animations; }
  const Animation *getAnimation(const std::string &name) const
  {
    auto it = animations.find(name);
    if (it != animations.end())
    {
      return &it->second;
    }
    return nullptr;
  }

  // Clear all animations
  void clear()
  {
    animations.clear();
    currentAnimation = "";
    currentFrame = 0.0f;
    frameTimer = 0.0f;
    playing = false;
  }
};
