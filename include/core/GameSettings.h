#pragma once

#include <string>
#include <iostream>

enum class TextureFilter
{
  NEAREST,
  LINEAR
};

class GameSettings
{
public:
  // Window settings
  struct WindowSettings
  {
    int width;
    int height;
    std::string title;
    bool resizable;
    bool vsync;
    bool centerOnScreen;
    bool fullscreen;
  } window;

  // Graphics settings
  struct GraphicsSettings
  {
    int viewportWidth;
    int viewportHeight;
    float clearColorR;
    float clearColorG;
    float clearColorB;
    bool enableBlending;
    TextureFilter textureFiltering;
  } graphics;

  // Audio settings
  struct AudioSettings
  {
    float masterVolume;
    float sfxVolume;
    float musicVolume;
    bool muted;
  } audio;

  // Input settings
  struct InputSettings
  {
    float mouseSensitivity;
    bool invertMouse;
  } input;

  // Game settings
  struct GameplaySettings
  {
    int targetFPS;
    bool debugMode;
    bool showFPS;
  } game;

public:
  GameSettings()
  {
    // Values will be set by the caller (main.cpp)
  }

  ~GameSettings() = default;

  // Validation methods
  bool validateSettings()
  {
    bool valid = true;

    // Validate window settings
    if (window.width <= 0 || window.height <= 0)
    {
      std::cerr << "Invalid window dimensions" << std::endl;
      valid = false;
    }

    // Validate graphics settings
    if (graphics.viewportWidth <= 0 || graphics.viewportHeight <= 0)
    {
      std::cerr << "Invalid viewport dimensions" << std::endl;
      valid = false;
    }

    // Validate audio settings
    if (audio.masterVolume < 0.0f || audio.masterVolume > 1.0f)
    {
      std::cerr << "Invalid master volume" << std::endl;
      valid = false;
    }

    if (audio.sfxVolume < 0.0f || audio.sfxVolume > 1.0f)
    {
      std::cerr << "Invalid SFX volume" << std::endl;
      valid = false;
    }

    if (audio.musicVolume < 0.0f || audio.musicVolume > 1.0f)
    {
      std::cerr << "Invalid music volume" << std::endl;
      valid = false;
    }

    // Validate game settings
    if (game.targetFPS <= 0)
    {
      std::cerr << "Invalid target FPS" << std::endl;
      valid = false;
    }

    return valid;
  }

  void clampValues()
  {
    // Clamp window dimensions
    window.width = std::max(1, window.width);
    window.height = std::max(1, window.height);

    // Clamp viewport dimensions
    graphics.viewportWidth = std::max(1, graphics.viewportWidth);
    graphics.viewportHeight = std::max(1, graphics.viewportHeight);

    // Clamp color values
    graphics.clearColorR = std::max(0.0f, std::min(1.0f, graphics.clearColorR));
    graphics.clearColorG = std::max(0.0f, std::min(1.0f, graphics.clearColorG));
    graphics.clearColorB = std::max(0.0f, std::min(1.0f, graphics.clearColorB));

    // Clamp audio volumes
    audio.masterVolume = std::max(0.0f, std::min(1.0f, audio.masterVolume));
    audio.sfxVolume = std::max(0.0f, std::min(1.0f, audio.sfxVolume));
    audio.musicVolume = std::max(0.0f, std::min(1.0f, audio.musicVolume));

    // Clamp input sensitivity
    input.mouseSensitivity = std::max(0.1f, std::min(10.0f, input.mouseSensitivity));

    // Clamp target FPS
    game.targetFPS = std::max(1, std::min(1000, game.targetFPS));
  }

  // Convenience methods
  void setWindowSize(int width, int height)
  {
    window.width = width;
    window.height = height;
  }

  void setViewportSize(int width, int height)
  {
    graphics.viewportWidth = width;
    graphics.viewportHeight = height;
  }

  void setClearColor(float r, float g, float b)
  {
    graphics.clearColorR = r;
    graphics.clearColorG = g;
    graphics.clearColorB = b;
  }

  // Debug output
  void printSettings() const
  {
    std::cout << "=== Game Settings ===" << std::endl;
    std::cout << "Window: " << window.width << "x" << window.height << " (" << window.title << ")" << std::endl;
    std::cout << "Viewport: " << graphics.viewportWidth << "x" << graphics.viewportHeight << std::endl;
    std::cout << "Clear Color: (" << graphics.clearColorR << ", " << graphics.clearColorG << ", " << graphics.clearColorB << ")" << std::endl;
    std::cout << "Target FPS: " << game.targetFPS << std::endl;
    std::cout << "Debug Mode: " << (game.debugMode ? "ON" : "OFF") << std::endl;
    std::cout << "====================" << std::endl;
  }
};
