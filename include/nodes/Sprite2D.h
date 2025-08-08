#pragma once

#include "Node2D.h"
#include "../core/GameSettings.h"
#include "Animation2D.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <memory>

// Forward declaration for texture data
struct TextureData;

class Sprite2D : public Node2D
{
private:
  std::string imagePath;
  std::unique_ptr<TextureData> textureData;
  bool textureLoaded;
  Color tintColor;
  bool useTint;

  // Sprite sheet animation properties
  int hframes; // Number of horizontal frames in the sprite sheet
  int vframes; // Number of vertical frames in the sprite sheet
  int frame;   // Current frame index (0-based)

  // Animation system
  std::unique_ptr<Animation2D> animator;

public:
  Sprite2D(const std::string &nodeName = "Sprite2D",
           const Position2D &pos = Position2D(),
           const Scale2D &scale = Scale2D(1.0f, 1.0f),
           const std::string &imagePath = "",
           int hframes = 1,
           int vframes = 1)
      : Node2D(nodeName), imagePath(imagePath), textureLoaded(false),
        tintColor(Colors::white), useTint(false),
        hframes(hframes), vframes(vframes), frame(0)
  {
    setPosition(pos);
    setScale(scale);
    animator = std::make_unique<Animation2D>();
    std::cout << "Sprite2D constructor called with imagePath: " << imagePath << std::endl;
    if (!imagePath.empty())
    {
      loadTexture(imagePath);
    }
  }

  // Constructors with individual parameters for convenience
  Sprite2D(const std::string &nodeName,
           float x, float y,
           float scaleX = 1.0f, float scaleY = 1.0f,
           const std::string &imgPath = "",
           int hframes = 1,
           int vframes = 1)
      : Node2D(nodeName), imagePath(imgPath), textureLoaded(false),
        tintColor(Colors::white), useTint(false),
        hframes(hframes), vframes(vframes), frame(0)
  {
    setPosition(Position2D(x, y));
    setScale(Scale2D(scaleX, scaleY));
    animator = std::make_unique<Animation2D>();
    if (!imgPath.empty())
    {
      loadTexture(imgPath);
    }
  }

  // Destructor to clean up OpenGL resources
  ~Sprite2D();

  // Image management
  bool loadTexture(const std::string &path, TextureFilter filter = TextureFilter::NEAREST);
  bool isTextureLoaded() const { return textureLoaded; }
  const std::string &getImagePath() const { return imagePath; }

  // Tint color management
  void setTint(const Color &color)
  {
    tintColor = color;
    useTint = true;
  }
  void setTint(float r, float g, float b)
  {
    tintColor = Color(r, g, b);
    useTint = true;
  }
  void clearTint() { useTint = false; }
  const Color &getTint() const { return tintColor; }
  bool hasTint() const { return useTint; }

  // Animation properties
  int getHFrames() const { return hframes; }
  int getVFrames() const { return vframes; }
  int getFrame() const { return frame; }
  int getTotalFrames() const { return hframes * vframes; }

  void setHFrames(int h) { hframes = h > 0 ? h : 1; }
  void setVFrames(int v) { vframes = v > 0 ? v : 1; }
  void setFrame(int f) { frame = f % (hframes * vframes); }

  // Animation system methods
  Animation2D *getAnimator() { return animator.get(); }
  const Animation2D *getAnimator() const { return animator.get(); }

  // Update animation (call this in your game loop)
  void updateAnimation(float deltaTime);

  // Play animation by name
  void playAnimation(const std::string &animationName);

  // Stop current animation
  void stopAnimation();

  // Get current animation name
  std::string getCurrentAnimation() const;

  // Render the sprite
  void render() const override;
};

// Texture data structure
struct TextureData
{
  unsigned int textureId;
  int width;
  int height;
  int channels;

  TextureData() : textureId(0), width(0), height(0), channels(0) {}
  ~TextureData();
};