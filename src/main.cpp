#include "core/Window.h"
#include "core/GameSettings.h"
#include "core/Camera.h"
#include "scene/Scene.h"
#include "scene/SceneLoader.h"
#include "nodes/Sprite2D.h"
#include <GL/gl.h>
#include <iostream>
#include <vector>
#include <chrono>

int main(void)
{
  // Load game settings
  GameSettings settings;
  settings.printSettings();

  // Create window with settings from config
  WindowConfig config;
  config.width = settings.window.width;
  config.height = settings.window.height;
  config.title = settings.window.title;
  config.resizable = settings.window.resizable;
  config.centerOnScreen = settings.window.centerOnScreen;

  Window window(config, &settings);

  if (!window.isValid())
  {
    std::cerr << "Failed to create window" << std::endl;
    return -1;
  }

  // Ensure OpenGL context is current and set up basic state
  glfwMakeContextCurrent(window.getGLFWWindow());

  // Check if OpenGL is working
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
  {
    std::cerr << "OpenGL error after context setup: " << error << std::endl;
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  // Create camera with viewport dimensions from settings
  Camera camera(settings.graphics.viewportWidth, settings.graphics.viewportHeight);
  camera.printInfo();

  // Print camera bounds to see what's visible
  std::cout << "Camera view bounds:" << std::endl;
  std::cout << "  Left: " << camera.getLeft() << ", Right: " << camera.getRight() << std::endl;
  std::cout << "  Top: " << camera.getTop() << ", Bottom: " << camera.getBottom() << std::endl;
  std::cout << "  Sprite at (160,90) is visible: " << (camera.isVisible(Position2D(160, 90)) ? "yes" : "no") << std::endl;

  // Get available scene files
  auto availableScenes = SceneLoader::getAvailableScenes();
  std::cout << "Available scenes:" << std::endl;
  for (const auto &sceneFile : availableScenes)
  {
    std::cout << "  - " << sceneFile << std::endl;
  }
  std::cout << std::endl;

  // Load the aliens demo scene from YAML file
  Scene currentScene = SceneLoader::loadSceneFromYaml("scenes/aliens_demo.yaml");
  std::cout << "Loaded scene: " << currentScene.getName() << std::endl;
  std::cout << "Nodes in scene: " << currentScene.getNodeCount() << std::endl;

  // Display information about nodes in the scene
  auto rootNode = currentScene.getRoot();
  if (rootNode)
  {
    std::cout << "Nodes loaded from scene file:" << std::endl;
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

  // Timing variables
  auto lastTime = std::chrono::high_resolution_clock::now();
  float frameTimer = 0.0f;
  int currentFrame = 0;

  // Main render loop
  while (!window.shouldClose())
  {
    // Calculate delta time
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;

    // Update the scene (animations, etc.)
    currentScene.update(deltaTime);

    // Update animations for all Sprite2D nodes
    auto rootNode = currentScene.getRoot();
    if (rootNode)
    {
      for (auto &child : rootNode->getChildren())
      {
        if (auto sprite = dynamic_cast<Sprite2D *>(child.get()))
        {
          sprite->updateAnimation(deltaTime);
        }
      }
    }

    // Start playing animations after a short delay
    static bool animationsStarted = false;
    static float startTimer = 0.0f;
    if (!animationsStarted)
    {
      startTimer += deltaTime;
      if (startTimer >= 1.0f) // Start animations after 1 second
      {
        if (rootNode)
        {
          for (auto &child : rootNode->getChildren())
          {
            if (auto sprite = dynamic_cast<Sprite2D *>(child.get()))
            {
              // Check if IDLE animation exists before playing it
              if (sprite->getAnimator() && sprite->getAnimator()->hasAnimation("IDLE"))
              {
                sprite->playAnimation("IDLE");
                std::cout << "Started IDLE animation for sprite: " << sprite->getName() << std::endl;
              }
              else
              {
                std::cout << "No IDLE animation found for sprite: " << sprite->getName() << std::endl;
                // List available animations
                if (sprite->getAnimator())
                {
                  const auto &animations = sprite->getAnimator()->getAllAnimations();
                  std::cout << "Available animations: ";
                  for (const auto &anim : animations)
                  {
                    std::cout << anim.first << " ";
                  }
                  std::cout << std::endl;
                }
              }
            }
          }
        }
        animationsStarted = true;
      }
    }

    // Handle keyboard input for animation testing
    static bool keyPressed = false;
    if (window.isKeyPressed(GLFW_KEY_SPACE) && !keyPressed)
    {
      keyPressed = true;
      if (rootNode)
      {
        for (auto &child : rootNode->getChildren())
        {
          if (auto sprite = dynamic_cast<Sprite2D *>(child.get()))
          {
            // Cycle through different animations
            std::string currentAnim = sprite->getCurrentAnimation();
            if (currentAnim == "IDLE")
              sprite->playAnimation("WALK");
            else if (currentAnim == "WALK")
              sprite->playAnimation("RUN");
            else if (currentAnim == "RUN")
              sprite->playAnimation("JUMP");
            else if (currentAnim == "JUMP")
              sprite->playAnimation("PUNCH");
            else if (currentAnim == "PUNCH")
              sprite->playAnimation("KICK");
            else
              sprite->playAnimation("IDLE");
          }
        }
      }
    }
    else if (!window.isKeyPressed(GLFW_KEY_SPACE))
    {
      keyPressed = false;
    }

    // Clear the color buffer using settings
    glClearColor(settings.graphics.clearColorR, settings.graphics.clearColorG, settings.graphics.clearColorB, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the current scene (all nodes)
    // std::cout << "Rendering scene with " << currentScene.getNodeCount() << " nodes" << std::endl;
    currentScene.render();

    // Swap front and back buffers
    window.swapBuffers();

    // Poll for and process events
    window.pollEvents();
  }

  return 0;
}