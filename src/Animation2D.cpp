#include "../include/nodes/Animation2D.h"
#include <iostream>

Animation2D::Animation2D()
    : currentAnimation(""), currentFrame(0.0f), frameTimer(0.0f), playing(false)
{
}

void Animation2D::addAnimation(const std::string &name, const Animation &animation)
{
  animations[name] = animation;
  std::cout << "Added animation: " << name << " with " << animation.frames.size() << " frame groups" << std::endl;
}

void Animation2D::addAnimation(const std::string &name, const std::vector<AnimationFrame> &frames,
                               float frameRate, bool loop)
{
  Animation anim(name, frameRate, loop);
  anim.frames = frames;
  animations[name] = anim;
  std::cout << "Added animation: " << name << " with " << frames.size() << " frame groups" << std::endl;
}

void Animation2D::removeAnimation(const std::string &name)
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

bool Animation2D::hasAnimation(const std::string &name) const
{
  return animations.find(name) != animations.end();
}

void Animation2D::play(const std::string &animationName)
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

void Animation2D::stop()
{
  playing = false;
  currentFrame = 0.0f;
  frameTimer = 0.0f;
}

void Animation2D::pause()
{
  playing = false;
}

void Animation2D::resume()
{
  if (!currentAnimation.empty())
  {
    playing = true;
  }
}

void Animation2D::reset()
{
  currentFrame = 0.0f;
  frameTimer = 0.0f;
}

void Animation2D::update(float deltaTime)
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

int Animation2D::getCurrentFrameIndex() const
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

AnimationFrame Animation2D::getCurrentAnimationFrame() const
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

const Animation *Animation2D::getAnimation(const std::string &name) const
{
  auto it = animations.find(name);
  if (it != animations.end())
  {
    return &it->second;
  }
  return nullptr;
}

void Animation2D::clear()
{
  animations.clear();
  currentAnimation = "";
  currentFrame = 0.0f;
  frameTimer = 0.0f;
  playing = false;
}
