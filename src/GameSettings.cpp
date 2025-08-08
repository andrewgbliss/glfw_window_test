#include "../include/core/GameSettings.h"
#include <algorithm>
#include <cctype>

GameSettings::GameSettings(const std::string &configPath) : configFilePath(configPath)
{
  resetToDefaults();
  loadFromFile();
}

std::string GameSettings::trim(const std::string &str)
{
  size_t start = str.find_first_not_of(" \t\r\n");
  if (start == std::string::npos)
    return "";

  size_t end = str.find_last_not_of(" \t\r\n");
  return str.substr(start, end - start + 1);
}

bool GameSettings::parseBool(const std::string &value)
{
  std::string lower = value;
  std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
  return (lower == "true" || lower == "1" || lower == "yes" || lower == "on");
}

int GameSettings::parseInt(const std::string &value)
{
  try
  {
    return std::stoi(value);
  }
  catch (const std::exception &)
  {
    return 0;
  }
}

float GameSettings::parseFloat(const std::string &value)
{
  try
  {
    return std::stof(value);
  }
  catch (const std::exception &)
  {
    return 0.0f;
  }
}

TextureFilter GameSettings::parseTextureFilter(const std::string &value)
{
  std::string lower = value;
  std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

  if (lower == "linear")
    return TextureFilter::LINEAR;
  else
    return TextureFilter::NEAREST;
}

std::string GameSettings::textureFilterToString(TextureFilter filter) const
{
  switch (filter)
  {
  case TextureFilter::LINEAR:
    return "linear";
  case TextureFilter::NEAREST:
  default:
    return "nearest";
  }
}

bool GameSettings::loadFromFile()
{
  std::ifstream file(configFilePath);
  if (!file.is_open())
  {
    std::cout << "Config file not found: " << configFilePath << ", using defaults" << std::endl;
    return false;
  }

  iniData.clear();
  std::string line;
  std::string currentSection;

  while (std::getline(file, line))
  {
    line = trim(line);

    // Skip empty lines and comments
    if (line.empty() || line[0] == ';' || line[0] == '#')
      continue;

    // Check for section headers
    if (line[0] == '[' && line.back() == ']')
    {
      currentSection = line.substr(1, line.length() - 2);
      continue;
    }

    // Parse key=value pairs
    size_t equalPos = line.find('=');
    if (equalPos != std::string::npos && !currentSection.empty())
    {
      std::string key = trim(line.substr(0, equalPos));
      std::string value = trim(line.substr(equalPos + 1));
      iniData[currentSection][key] = value;
    }
  }

  file.close();

  // Apply loaded settings
  applyLoadedSettings();

  std::cout << "Loaded settings from: " << configFilePath << std::endl;
  return true;
}

void GameSettings::applyLoadedSettings()
{
  // Window settings
  if (iniData.count("Window"))
  {
    auto &windowData = iniData["Window"];
    if (windowData.count("width"))
      window.width = parseInt(windowData["width"]);
    if (windowData.count("height"))
      window.height = parseInt(windowData["height"]);
    if (windowData.count("title"))
      window.title = windowData["title"];
    if (windowData.count("resizable"))
      window.resizable = parseBool(windowData["resizable"]);
    if (windowData.count("vsync"))
      window.vsync = parseBool(windowData["vsync"]);
    if (windowData.count("center_on_screen"))
      window.centerOnScreen = parseBool(windowData["center_on_screen"]);
    if (windowData.count("fullscreen"))
      window.fullscreen = parseBool(windowData["fullscreen"]);
  }

  // Graphics settings
  if (iniData.count("Graphics"))
  {
    auto &graphicsData = iniData["Graphics"];
    if (graphicsData.count("viewport_width"))
      graphics.viewportWidth = parseInt(graphicsData["viewport_width"]);
    if (graphicsData.count("viewport_height"))
      graphics.viewportHeight = parseInt(graphicsData["viewport_height"]);
    if (graphicsData.count("clear_color_r"))
      graphics.clearColorR = parseFloat(graphicsData["clear_color_r"]);
    if (graphicsData.count("clear_color_g"))
      graphics.clearColorG = parseFloat(graphicsData["clear_color_g"]);
    if (graphicsData.count("clear_color_b"))
      graphics.clearColorB = parseFloat(graphicsData["clear_color_b"]);
    if (graphicsData.count("enable_blending"))
      graphics.enableBlending = parseBool(graphicsData["enable_blending"]);
    if (graphicsData.count("texture_filtering"))
      graphics.textureFiltering = parseTextureFilter(graphicsData["texture_filtering"]);
  }

  // Audio settings
  if (iniData.count("Audio"))
  {
    auto &audioData = iniData["Audio"];
    if (audioData.count("master_volume"))
      audio.masterVolume = parseFloat(audioData["master_volume"]);
    if (audioData.count("sfx_volume"))
      audio.sfxVolume = parseFloat(audioData["sfx_volume"]);
    if (audioData.count("music_volume"))
      audio.musicVolume = parseFloat(audioData["music_volume"]);
    if (audioData.count("muted"))
      audio.muted = parseBool(audioData["muted"]);
  }

  // Input settings
  if (iniData.count("Input"))
  {
    auto &inputData = iniData["Input"];
    if (inputData.count("mouse_sensitivity"))
      input.mouseSensitivity = parseFloat(inputData["mouse_sensitivity"]);
    if (inputData.count("invert_mouse"))
      input.invertMouse = parseBool(inputData["invert_mouse"]);
  }

  // Game settings
  if (iniData.count("Game"))
  {
    auto &gameData = iniData["Game"];
    if (gameData.count("target_fps"))
      game.targetFPS = parseInt(gameData["target_fps"]);
    if (gameData.count("debug_mode"))
      game.debugMode = parseBool(gameData["debug_mode"]);
    if (gameData.count("show_fps"))
      game.showFPS = parseBool(gameData["show_fps"]);
  }

  // Validate and clamp values
  clampValues();
}

bool GameSettings::saveToFile()
{
  std::ofstream file(configFilePath);
  if (!file.is_open())
  {
    std::cerr << "Failed to open config file for writing: " << configFilePath << std::endl;
    return false;
  }

  // Write Window section
  file << "[Window]\n";
  file << "width=" << window.width << "\n";
  file << "height=" << window.height << "\n";
  file << "title=" << window.title << "\n";
  file << "resizable=" << (window.resizable ? "true" : "false") << "\n";
  file << "vsync=" << (window.vsync ? "true" : "false") << "\n";
  file << "center_on_screen=" << (window.centerOnScreen ? "true" : "false") << "\n";
  file << "fullscreen=" << (window.fullscreen ? "true" : "false") << "\n\n";

  // Write Graphics section
  file << "[Graphics]\n";
  file << "viewport_width=" << graphics.viewportWidth << "\n";
  file << "viewport_height=" << graphics.viewportHeight << "\n";
  file << "clear_color_r=" << graphics.clearColorR << "\n";
  file << "clear_color_g=" << graphics.clearColorG << "\n";
  file << "clear_color_b=" << graphics.clearColorB << "\n";
  file << "enable_blending=" << (graphics.enableBlending ? "true" : "false") << "\n";
  file << "texture_filtering=" << textureFilterToString(graphics.textureFiltering) << "\n\n";

  // Write Audio section
  file << "[Audio]\n";
  file << "master_volume=" << audio.masterVolume << "\n";
  file << "sfx_volume=" << audio.sfxVolume << "\n";
  file << "music_volume=" << audio.musicVolume << "\n";
  file << "muted=" << (audio.muted ? "true" : "false") << "\n\n";

  // Write Input section
  file << "[Input]\n";
  file << "mouse_sensitivity=" << input.mouseSensitivity << "\n";
  file << "invert_mouse=" << (input.invertMouse ? "true" : "false") << "\n\n";

  // Write Game section
  file << "[Game]\n";
  file << "target_fps=" << game.targetFPS << "\n";
  file << "debug_mode=" << (game.debugMode ? "true" : "false") << "\n";
  file << "show_fps=" << (game.showFPS ? "true" : "false") << "\n";

  file.close();
  std::cout << "Saved settings to: " << configFilePath << std::endl;
  return true;
}

void GameSettings::resetToDefaults()
{
  window = WindowSettings{};
  graphics = GraphicsSettings{};
  audio = AudioSettings{};
  input = InputSettings{};
  game = GameplaySettings{};
}

bool GameSettings::validateSettings()
{
  bool valid = true;

  // Validate window dimensions
  if (window.width <= 0 || window.height <= 0)
  {
    std::cerr << "Invalid window dimensions: " << window.width << "x" << window.height << std::endl;
    valid = false;
  }

  // Validate viewport dimensions
  if (graphics.viewportWidth <= 0 || graphics.viewportHeight <= 0)
  {
    std::cerr << "Invalid viewport dimensions: " << graphics.viewportWidth << "x" << graphics.viewportHeight << std::endl;
    valid = false;
  }

  return valid;
}

void GameSettings::clampValues()
{
  // Clamp window dimensions
  window.width = std::max(320, std::min(7680, window.width));
  window.height = std::max(240, std::min(4320, window.height));

  // Clamp viewport dimensions
  graphics.viewportWidth = std::max(320, std::min(7680, graphics.viewportWidth));
  graphics.viewportHeight = std::max(240, std::min(4320, graphics.viewportHeight));

  // Clamp color values
  graphics.clearColorR = std::max(0.0f, std::min(1.0f, graphics.clearColorR));
  graphics.clearColorG = std::max(0.0f, std::min(1.0f, graphics.clearColorG));
  graphics.clearColorB = std::max(0.0f, std::min(1.0f, graphics.clearColorB));

  // Clamp audio values
  audio.masterVolume = std::max(0.0f, std::min(1.0f, audio.masterVolume));
  audio.sfxVolume = std::max(0.0f, std::min(1.0f, audio.sfxVolume));
  audio.musicVolume = std::max(0.0f, std::min(1.0f, audio.musicVolume));

  // Clamp input values
  input.mouseSensitivity = std::max(0.1f, std::min(10.0f, input.mouseSensitivity));

  // Clamp game values
  game.targetFPS = std::max(30, std::min(300, game.targetFPS));
}

void GameSettings::setWindowSize(int width, int height)
{
  window.width = width;
  window.height = height;
}

void GameSettings::setViewportSize(int width, int height)
{
  graphics.viewportWidth = width;
  graphics.viewportHeight = height;
}

void GameSettings::setClearColor(float r, float g, float b)
{
  graphics.clearColorR = r;
  graphics.clearColorG = g;
  graphics.clearColorB = b;
}

void GameSettings::printSettings() const
{
  std::cout << "=== Game Settings ===" << std::endl;
  std::cout << "Window: " << window.width << "x" << window.height << " '" << window.title << "'" << std::endl;
  std::cout << "Viewport: " << graphics.viewportWidth << "x" << graphics.viewportHeight << std::endl;
  std::cout << "Clear Color: (" << graphics.clearColorR << ", " << graphics.clearColorG << ", " << graphics.clearColorB << ")" << std::endl;
  std::cout << "VSync: " << (window.vsync ? "ON" : "OFF") << std::endl;
  std::cout << "Texture Filtering: " << textureFilterToString(graphics.textureFiltering) << std::endl;
  std::cout << "Target FPS: " << game.targetFPS << std::endl;
  std::cout << "Debug Mode: " << (game.debugMode ? "ON" : "OFF") << std::endl;
  std::cout << "====================" << std::endl;
}
