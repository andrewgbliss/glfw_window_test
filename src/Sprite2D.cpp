#include "../include/nodes/Sprite2D.h"
#include "../include/core/GameSettings.h"
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>
#include <cstring>
#include <fstream>

// Define STB_IMAGE_IMPLEMENTATION before including stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"

// TextureData destructor
TextureData::~TextureData()
{
  if (textureId != 0)
  {
    glDeleteTextures(1, &textureId);
    textureId = 0;
  }
}

// Sprite2D destructor
Sprite2D::~Sprite2D()
{
  // The unique_ptr will automatically clean up the TextureData
}

bool Sprite2D::loadTexture(const std::string &path, TextureFilter filter)
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

  // Generate OpenGL texture
  glGenTextures(1, &textureData->textureId);
  glBindTexture(GL_TEXTURE_2D, textureData->textureId);

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  // Set filtering based on parameter
  GLenum filterMode = (filter == TextureFilter::LINEAR) ? GL_LINEAR : GL_NEAREST;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

  // Upload texture data
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  // Free the image data
  stbi_image_free(data);

  textureLoaded = true;
  imagePath = path;

  std::cout << "Successfully loaded texture: " << path << " (" << width << "x" << height << ")" << std::endl;
  std::cout << "Texture ID: " << textureData->textureId << std::endl;
  std::cout << "HFrames: " << hframes << ", VFrames: " << vframes << std::endl;
  return true;
}

void Sprite2D::render() const
{

  if (!textureLoaded || !textureData)
  {
    std::cout << "Rendering placeholder - texture not loaded" << std::endl;
    // If no texture is loaded, render a placeholder colored rectangle
    glPushMatrix();

    // Apply transformations
    const Position2D &pos = getTransform().getPosition();
    const Scale2D &scale = getTransform().getScale();
    float rotation = getTransform().getRotation();

    glTranslatef(pos.x, pos.y, 0.0f);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glScalef(scale.x, scale.y, 1.0f);

    // Set color for placeholder - make it green to show it's working
    glColor3f(0.0f, 1.0f, 0.0f);

    // Draw placeholder rectangle
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.0f);
    glEnd();

    glPopMatrix();
    return;
  }

  // Enable texture mapping
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);

  glBindTexture(GL_TEXTURE_2D, textureData->textureId);

  glPushMatrix();

  // Apply transformations
  const Position2D &pos = getTransform().getPosition();
  const Scale2D &scale = getTransform().getScale();
  float rotation = getTransform().getRotation();

  glTranslatef(pos.x, pos.y, 0.0f);
  glRotatef(rotation, 0.0f, 0.0f, 1.0f);
  glScalef(scale.x, scale.y, 1.0f);

  // Set color (tint if specified)
  if (useTint)
  {
    glColor3f(tintColor.x, tintColor.y, tintColor.z);
  }
  else
  {
    glColor3f(1.0f, 1.0f, 1.0f); // White (no tint)
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

  // Calculate half dimensions for centered rendering
  float halfWidth = (static_cast<float>(textureData->width) / hframes) / 2.0f;
  float halfHeight = (static_cast<float>(textureData->height) / vframes) / 2.0f;

  // Draw textured quad with frame coordinates

  glBegin(GL_QUADS);
  glTexCoord2f(texLeft, texTop);
  glVertex3f(-halfWidth, halfHeight, 0.0f); // Top-left
  glTexCoord2f(texRight, texTop);
  glVertex3f(halfWidth, halfHeight, 0.0f); // Top-right
  glTexCoord2f(texRight, texBottom);
  glVertex3f(halfWidth, -halfHeight, 0.0f); // Bottom-right
  glTexCoord2f(texLeft, texBottom);
  glVertex3f(-halfWidth, -halfHeight, 0.0f); // Bottom-left
  glEnd();

  glPopMatrix();

  // Restore OpenGL state
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}

// Animation system methods
void Sprite2D::updateAnimation(float deltaTime)
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

void Sprite2D::playAnimation(const std::string &animationName)
{
  if (animator)
  {
    animator->play(animationName);
  }
}

void Sprite2D::stopAnimation()
{
  if (animator)
  {
    animator->stop();
  }
}

std::string Sprite2D::getCurrentAnimation() const
{
  if (animator)
  {
    return animator->getCurrentAnimation();
  }
  return "";
}