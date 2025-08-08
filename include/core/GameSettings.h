#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

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
    int width = 1280;
    int height = 720;
    std::string title = "OpenGL Scene Loader Demo";
    bool resizable = true;
    bool vsync = true;
    bool centerOnScreen = true;
    bool fullscreen = false;
  } window;

  // Graphics settings
  struct GraphicsSettings
  {
    int viewportWidth = 1280;
    int viewportHeight = 720;
    float clearColorR = 0.1f;
    float clearColorG = 0.1f;
    float clearColorB = 0.1f;
    bool enableBlending = true;
    TextureFilter textureFiltering = TextureFilter::NEAREST;
  } graphics;

  // Audio settings
  struct AudioSettings
  {
    float masterVolume = 1.0f;
    float sfxVolume = 0.8f;
    float musicVolume = 0.6f;
    bool muted = false;
  } audio;

  // Input settings
  struct InputSettings
  {
    float mouseSensitivity = 1.0f;
    bool invertMouse = false;
  } input;

  // Game settings
  struct GameplaySettings
  {
    int targetFPS = 60;
    bool debugMode = false;
    bool showFPS = false;
  } game;

private:
  std::string configFilePath;
  std::unordered_map<std::string, std::unordered_map<std::string, std::string>> iniData;

  // Helper methods for parsing
  std::string trim(const std::string &str);
  bool parseBool(const std::string &value);
  int parseInt(const std::string &value);
  float parseFloat(const std::string &value);
  TextureFilter parseTextureFilter(const std::string &value);
  std::string textureFilterToString(TextureFilter filter) const;
  void applyLoadedSettings();

public:
  GameSettings(const std::string &configPath = "config.ini");
  ~GameSettings() = default;

  // Load settings from file
  bool loadFromFile();

  // Save settings to file
  bool saveToFile();

  // Get the config file path
  const std::string &getConfigPath() const { return configFilePath; }

  // Reload settings from file
  bool reload() { return loadFromFile(); }

  // Reset to default values
  void resetToDefaults();

  // Validation methods
  bool validateSettings();
  void clampValues();

  // Convenience methods
  void setWindowSize(int width, int height);
  void setViewportSize(int width, int height);
  void setClearColor(float r, float g, float b);

  // Debug output
  void printSettings() const;
};
