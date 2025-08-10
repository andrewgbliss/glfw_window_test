#pragma once

#include "GameSettings.h"
#include "window/OpenGLWindow.h"
#include "render/RenderDevice.h"
#include "render/OpenGLRenderDriver.h"
#include "Camera.h"
#include "Input.h"
#include "../scene/Scene.h"
#include "../scene/MinimalScene.h"
#include "../scene/SimpleScene.h"
#include "../scene/ColorfulScene.h"
#include "../scene/AliensDemoScene.h"
#include "../scene/TestScene.h"
#include "../nodes/Sprite2D.h"
#include <memory>
#include <string>
#include <iostream>

class GameSetup
{
private:
  GameSettings settings;
  std::unique_ptr<OpenGLWindow> window;
  Camera camera;
  Scene currentScene;
  bool initialized;

public:
  GameSetup()
      : initialized(false) {}

  ~GameSetup() = default;

  // Initialize all systems
  bool initialize();

  // Load a specific scene
  bool loadScene(Scene scene);

  // Get access to components
  GameSettings &getSettings() { return settings; }
  OpenGLWindow *getWindow() { return window.get(); }
  Camera &getCamera() { return camera; }
  Scene &getCurrentScene() { return currentScene; }

  // Check if everything is properly initialized
  bool isInitialized() const { return initialized; }

  // Print debug information
  void printDebugInfo() const;

  // Update input system (call this each frame)
  void updateInput();

private:
  // Initialize window
  bool initializeWindow();

  // Initialize render device
  bool initializeRenderDevice();

  // Initialize camera
  bool initializeCamera();

  // Initialize input system
  bool initializeInput();

  // Print available scenes
  void printAvailableScenes() const;
};

// Implementation
inline bool GameSetup::initialize()
{
  std::cout << "=== Game Setup Initialization ===" << std::endl;

  // Print settings
  settings.printSettings();

  // Initialize window
  if (!initializeWindow())
  {
    std::cerr << "Failed to initialize window" << std::endl;
    return false;
  }

  // Initialize render device
  if (!initializeRenderDevice())
  {
    std::cerr << "Failed to initialize render device" << std::endl;
    return false;
  }

  // Initialize camera
  if (!initializeCamera())
  {
    std::cerr << "Failed to initialize camera" << std::endl;
    return false;
  }

  // Initialize input system
  if (!initializeInput())
  {
    std::cerr << "Failed to initialize input system" << std::endl;
    return false;
  }

  // Print available scenes
  printAvailableScenes();

  initialized = true;
  std::cout << "=== Game Setup Complete ===" << std::endl;
  return true;
}

inline bool GameSetup::initializeWindow()
{
  // Create window with settings from config
  WindowConfig config;
  config.width = settings.window.width;
  config.height = settings.window.height;
  config.title = settings.window.title;
  config.resizable = settings.window.resizable;
  config.centerOnScreen = settings.window.centerOnScreen;

  window = std::make_unique<OpenGLWindow>(config, &settings);

  if (!window->isValid())
  {
    std::cerr << "Failed to create window" << std::endl;
    return false;
  }

  std::cout << "Window initialized successfully" << std::endl;
  return true;
}

inline bool GameSetup::initializeRenderDevice()
{
  // Initialize render device with OpenGL driver
  auto &renderDevice = RenderDevice::getInstance();
  renderDevice.setDriver(std::make_unique<OpenGLRenderDriver>());

  if (!renderDevice.initialize(window.get()))
  {
    std::cerr << "Failed to initialize render device" << std::endl;
    return false;
  }

  // Set up 2D rendering
  renderDevice.setup2DRendering(settings.graphics.viewportWidth, settings.graphics.viewportHeight);
  renderDevice.clear(settings.graphics.clearColorR, settings.graphics.clearColorG, settings.graphics.clearColorB, 1.0f);

  std::cout << "Render device initialized successfully" << std::endl;
  return true;
}

inline bool GameSetup::initializeCamera()
{
  // Create camera with viewport dimensions from settings
  camera = Camera(settings.graphics.viewportWidth, settings.graphics.viewportHeight);
  camera.printInfo();

  // Print camera bounds to see what's visible
  std::cout << "Camera view bounds:" << std::endl;
  std::cout << "  Left: " << camera.getLeft() << ", Right: " << camera.getRight() << std::endl;
  std::cout << "  Top: " << camera.getTop() << ", Bottom: " << camera.getBottom() << std::endl;
  std::cout << "  Sprite at (160,90) is visible: " << (camera.isVisible(Position2D(160, 90)) ? "yes" : "no") << std::endl;

  std::cout << "Camera initialized successfully" << std::endl;
  return true;
}

inline bool GameSetup::initializeInput()
{
  auto &input = Input::getInstance();

  std::cout << "Input system initialized successfully" << std::endl;
  return true;
}

inline void GameSetup::updateInput()
{
  if (!window || !initialized)
    return;

  auto &input = Input::getInstance();

  // Update the input system (store previous frame states)
  input.update();

  // Debug: Check if any keys are being pressed
  bool anyKeyPressed = false;
  for (int key = 32; key <= 90; key++) // Common key range (space to Z)
  {
    bool isPressed = window->isKeyPressed(key);
    if (isPressed)
    {
      anyKeyPressed = true;
      std::cout << "GLFW Key " << key << " is pressed" << std::endl;
    }
    input.setKeyState(key, isPressed);
  }

  // Handle modifier keys
  bool shiftPressed = window->isKeyPressed(340);
  bool ctrlPressed = window->isKeyPressed(341);
  bool altPressed = window->isKeyPressed(342);

  if (shiftPressed || ctrlPressed || altPressed)
  {
    std::cout << "Modifier keys - Shift: " << shiftPressed
              << ", Ctrl: " << ctrlPressed
              << ", Alt: " << altPressed << std::endl;
  }

  input.setKeyState(340, shiftPressed); // Left Shift
  input.setKeyState(341, ctrlPressed);  // Left Control
  input.setKeyState(342, altPressed);   // Left Alt

  if (anyKeyPressed)
  {
    std::cout << "Some keys are being pressed!" << std::endl;
  }
}

inline bool GameSetup::loadScene(Scene scene)
{
  if (!initialized)
  {
    std::cerr << "GameSetup not initialized. Call initialize() first." << std::endl;
    return false;
  }

  std::cout << "Loading scene: " << scene.getName() << std::endl;

  // Move the scene
  currentScene = std::move(scene);

  std::cout << "Loaded scene: " << currentScene.getName() << std::endl;
  std::cout << "Nodes in scene: " << currentScene.getNodeCount() << std::endl;

  // Display information about nodes in the scene
  auto rootNode = currentScene.getRoot();
  if (rootNode)
  {
    std::cout << "Nodes loaded from scene:" << std::endl;
    for (const auto &child : rootNode->getChildren())
    {
      std::cout << "  - " << child->getName() << " (" << typeid(*child).name() << ")" << std::endl;

      // Check if it's a Sprite2D and get more details
      if (auto sprite = dynamic_cast<Sprite2D *>(child.get()))
      {
        std::cout << "    Position: (" << sprite->getPosition().x << ", " << sprite->getPosition().y << ")" << std::endl;
        std::cout << "    Scale: (" << sprite->getScale().x << ", " << sprite->getScale().y << ")" << std::endl;
        std::cout << "    Has texture: " << (sprite->isTextureLoaded() ? "yes" : "no") << std::endl;
        std::cout << "    Current frame: " << sprite->getFrame() << std::endl;
        std::cout << "    HFrames: " << sprite->getHFrames() << ", VFrames: " << sprite->getVFrames() << std::endl;
        std::cout << "    Has animator: " << (sprite->getAnimator() ? "yes" : "no") << std::endl;
      }
    }
  }
  std::cout << std::endl;

  return true;
}

inline void GameSetup::printAvailableScenes() const
{
  std::cout << "Available scenes:" << std::endl;
  std::cout << "  - MinimalScene (minimal)" << std::endl;
  std::cout << "  - SimpleScene (simple)" << std::endl;
  std::cout << "  - ColorfulScene (colorful)" << std::endl;
  std::cout << "  - AliensDemo (aliens_demo)" << std::endl;
  std::cout << "  - TestScene (test)" << std::endl;
  std::cout << std::endl;
}

inline void GameSetup::printDebugInfo() const
{
  std::cout << "=== Game Setup Debug Info ===" << std::endl;
  std::cout << "Initialized: " << (initialized ? "Yes" : "No") << std::endl;
  std::cout << "Window valid: " << (window && window->isValid() ? "Yes" : "No") << std::endl;
  std::cout << "Render device initialized: " << (RenderDevice::getInstance().isInitialized() ? "Yes" : "No") << std::endl;
  std::cout << "Current scene: " << currentScene.getName() << " (" << currentScene.getNodeCount() << " nodes)" << std::endl;
  std::cout << "=============================" << std::endl;
}
