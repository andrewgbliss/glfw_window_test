#pragma once

#include "GameSetup.h"
#include "render/RenderDevice.h"
#include "Input.h"
#include <chrono>
#include <iostream>

class GameLoop
{
private:
  GameSetup &gameSetup;
  std::chrono::high_resolution_clock::time_point lastTime;

public:
  GameLoop(GameSetup &setup)
      : gameSetup(setup)
  {
    // Input system will be configured in main.cpp
  }

  ~GameLoop() = default;

  // Run the main game loop
  void run();

  // Handle input
  void handleInput();

  // Render the current frame
  void render();

private:
  // Calculate delta time
  float calculateDeltaTime();
};

// Implementation
inline void GameLoop::run()
{
  if (!gameSetup.isInitialized())
  {
    std::cerr << "GameSetup not initialized. Cannot run game loop." << std::endl;
    return;
  }

  auto window = gameSetup.getWindow();
  if (!window)
  {
    std::cerr << "No window available. Cannot run game loop." << std::endl;
    return;
  }

  std::cout << "Starting game loop..." << std::endl;

  // Main render loop
  while (!window->shouldClose())
  {
    // Calculate delta time
    float deltaTime = calculateDeltaTime();

    // Update the scene (animations, etc.)
    gameSetup.getCurrentScene().update(deltaTime);

    // Update input system (poll GLFW and update Input class)
    gameSetup.updateInput();

    // Handle keyboard input
    handleInput();

    // Handle scene-specific input
    gameSetup.getCurrentScene().handleInput();

    // Handle input for all nodes in the scene
    gameSetup.getCurrentScene().getRoot()->handleInputRecursive();

    // Render the current frame
    render();

    // Swap front and back buffers
    window->swapBuffers();

    // Poll for and process events
    window->pollEvents();
  }

  std::cout << "Game loop ended." << std::endl;
}

inline void GameLoop::handleInput()
{
  // Handle any game-level input here
  // Animation-specific input should be handled by the scene or individual nodes
}

inline void GameLoop::render()
{
  auto &renderDevice = RenderDevice::getInstance();

  // Clear the screen
  renderDevice.clear(
      gameSetup.getSettings().graphics.clearColorR,
      gameSetup.getSettings().graphics.clearColorG,
      gameSetup.getSettings().graphics.clearColorB,
      1.0f);

  // Render the current scene
  gameSetup.getCurrentScene().render();
}

inline float GameLoop::calculateDeltaTime()
{
  auto currentTime = std::chrono::high_resolution_clock::now();
  float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
  lastTime = currentTime;
  return deltaTime;
}
