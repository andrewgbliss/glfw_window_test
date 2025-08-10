#include "core/GameSetup.h"
#include "core/GameLoop.h"
#include "core/Input.h"
#include "scene/MinimalScene.h"
#include <iostream>

int main(void)
{
  // Create and initialize game setup
  GameSetup gameSetup;

  // Set default settings (matching the original config.ini values)
  auto &settings = gameSetup.getSettings();

  // Window settings
  settings.window.width = 1280;
  settings.window.height = 720;
  settings.window.title = "OpenGL Scene Loader Demo";
  settings.window.resizable = true;
  settings.window.vsync = true;
  settings.window.centerOnScreen = true;
  settings.window.fullscreen = false;

  // Graphics settings
  settings.graphics.viewportWidth = 320;
  settings.graphics.viewportHeight = 180;
  settings.graphics.clearColorR = 0.1f;
  settings.graphics.clearColorG = 0.1f;
  settings.graphics.clearColorB = 0.1f;
  settings.graphics.enableBlending = true;
  settings.graphics.textureFiltering = TextureFilter::NEAREST;

  // Audio settings
  settings.audio.masterVolume = 1.0f;
  settings.audio.sfxVolume = 0.8f;
  settings.audio.musicVolume = 0.6f;
  settings.audio.muted = false;

  // Input settings
  settings.input.mouseSensitivity = 1.0f;
  settings.input.invertMouse = false;

  // Game settings
  settings.game.targetFPS = 60;
  settings.game.debugMode = false;
  settings.game.showFPS = false;

  if (!gameSetup.initialize())
  {
    std::cerr << "Failed to initialize game setup" << std::endl;
    return -1;
  }

  // Setup key bindings for movement and actions
  auto &input = Input::getInstance();
  input.setupAction("move_left", 65);  // A key
  input.setupAction("move_right", 68); // D key
  input.setupAction("move_up", 87);    // W key
  input.setupAction("move_down", 83);  // S key
  input.setupAction("jump", 32);       // Spacebar
  input.setupAction("punch", 74);      // J key
  input.setupAction("kick", 75);       // K key
  input.setupAction("crouch", 76);     // L key

  std::cout << "Key bindings configured:" << std::endl;
  std::cout << "  W - move_up" << std::endl;
  std::cout << "  A - move_left" << std::endl;
  std::cout << "  S - move_down" << std::endl;
  std::cout << "  D - move_right" << std::endl;
  std::cout << "  Spacebar - jump" << std::endl;
  std::cout << "  J - punch" << std::endl;
  std::cout << "  K - kick" << std::endl;
  std::cout << "  L - crouch" << std::endl;
  std::cout << "  Shift - run (hold while moving)" << std::endl;

  // Load the aliens demo scene
  MinimalScene scene;
  if (!gameSetup.loadScene(std::move(scene)))
  {
    std::cerr << "Failed to load scene" << std::endl;
    return -1;
  }

  // Create and run the game loop
  GameLoop gameLoop(gameSetup);
  gameLoop.run();

  return 0;
}