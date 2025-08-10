#pragma once

#include "Node2D.h"
#include "../core/GameSettings.h"
#include "../core/render/RenderDevice.h"
#include "Animation2D.h"
#include <string>
#include <memory>
#include <iostream>
#include <cstring>
#include <fstream>

// Define STB_IMAGE_IMPLEMENTATION before including stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"

// Texture data structure
struct TextureData
{
  unsigned int textureId;
  int width;
  int height;
  int channels;

  TextureData() : textureId(0), width(0), height(0), channels(0) {}
  ~TextureData()
  {
    if (textureId != 0)
    {
      auto &renderDevice = RenderDevice::getInstance();
      renderDevice.deleteTexture(textureId);
      textureId = 0;
    }
  }
};

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
  ~Sprite2D()
  {
    // The unique_ptr will automatically clean up the TextureData
  }

  // Image management
  bool loadTexture(const std::string &path, TextureFilter filter = TextureFilter::NEAREST)
  {
    // Clean up existing texture
    textureData.reset();
    textureLoaded = false;

    std::cout << "Attempting to load texture: " << path << std::endl;

    // Load image using stb_image
    int width, height, channels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 4);

    if (!data)
    {
      std::cerr << "Failed to load image: " << path << std::endl;
      std::cerr << "stb_image error: " << stbi_failure_reason() << std::endl;

      // Check if file exists
      std::ifstream file(path);
      if (file.good())
      {
        std::cerr << "File exists but stb_image failed to load it" << std::endl;
      }
      else
      {
        std::cerr << "File does not exist or cannot be opened" << std::endl;
      }
      file.close();

      return false;
    }

    // Create texture data
    textureData = std::make_unique<TextureData>();
    textureData->width = width;
    textureData->height = height;
    textureData->channels = 4; // We force 4 channels (RGBA)

    // Generate texture using render device
    auto &renderDevice = RenderDevice::getInstance();
    textureData->textureId = renderDevice.createTexture();

    // Upload texture data with filtering
    bool useLinearFiltering = (filter == TextureFilter::LINEAR);
    renderDevice.uploadTexture(textureData->textureId, width, height, data, useLinearFiltering);

    // Free the image data
    stbi_image_free(data);

    textureLoaded = true;
    imagePath = path;

    std::cout << "Successfully loaded texture: " << path << " (" << width << "x" << height << ")" << std::endl;
    std::cout << "Texture ID: " << textureData->textureId << std::endl;
    std::cout << "HFrames: " << hframes << ", VFrames: " << vframes << std::endl;
    return true;
  }
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
  void updateAnimation(float deltaTime)
  {
    if (animator)
    {
      animator->update(deltaTime);
      // Update the current frame based on animation
      int newFrame = animator->getCurrentFrameIndex();
      if (newFrame != frame)
      {
        std::cout << "Frame changed from " << frame << " to " << newFrame << " for " << getName() << std::endl;
        frame = newFrame;
      }
    }
  }

  // Play animation by name
  void playAnimation(const std::string &animationName)
  {
    if (animator)
    {
      animator->play(animationName);
    }
  }

  // Stop current animation
  void stopAnimation()
  {
    if (animator)
    {
      animator->stop();
    }
  }

  // Get current animation name
  std::string getCurrentAnimation() const
  {
    if (animator)
    {
      return animator->getCurrentAnimation();
    }
    return "";
  }

  // Override update to handle animations
  void update(float deltaTime = 0.0f) override
  {
    // Call base class update
    Node2D::update(deltaTime);

    // Update animations
    updateAnimation(deltaTime);
  }

  // Render the sprite
  void render() const override
  {
    auto &renderDevice = RenderDevice::getInstance();

    if (!textureLoaded || !textureData)
    {
      std::cout << "Rendering placeholder - texture not loaded" << std::endl;
      // If no texture is loaded, render a placeholder colored rectangle

      // Apply transformations
      const Position2D &pos = getTransform().getPosition();
      const Scale2D &scale = getTransform().getScale();
      float rotation = getTransform().getRotation();

      renderDevice.setTransform(pos.x, pos.y, rotation, scale.x, scale.y);

      // Set color for placeholder - make it green to show it's working
      renderDevice.setColor(0.0f, 1.0f, 0.0f);

      // Draw placeholder rectangle
      renderDevice.drawRectangle(-0.5f, -0.5f, 1.0f, 1.0f);

      renderDevice.resetTransform();
      return;
    }

    // Apply transformations
    const Position2D &pos = getTransform().getPosition();
    const Scale2D &scale = getTransform().getScale();
    float rotation = getTransform().getRotation();

    renderDevice.setTransform(pos.x, pos.y, rotation, scale.x, scale.y);

    // Set color (tint if specified)
    if (useTint)
    {
      renderDevice.setColor(tintColor.x, tintColor.y, tintColor.z);
    }
    else
    {
      renderDevice.setColor(1.0f, 1.0f, 1.0f); // White (no tint)
    }

    // Calculate frame coordinates
    float frameWidth = 1.0f / hframes;
    float frameHeight = 1.0f / vframes;

    // Calculate frame position in sprite sheet
    int frameX = frame % hframes;
    int frameY = (frame / hframes) % vframes;

    // Calculate texture coordinates for current frame
    float texLeft = frameX * frameWidth;
    float texRight = texLeft + frameWidth;
    float texTop = 1.0f - (frameY * frameHeight); // Invert Y-axis
    float texBottom = texTop - frameHeight;

    // Calculate dimensions for centered rendering
    float width = static_cast<float>(textureData->width) / hframes;
    float height = static_cast<float>(textureData->height) / vframes;

    // Draw sprite with texture coordinates
    renderDevice.drawSprite(-width / 2, -height / 2, width, height, textureData->textureId, texLeft, texTop, texRight, texBottom);

    renderDevice.resetTransform();
  }
};