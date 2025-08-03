#include "Window.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Scene.h"
#include "SceneLoader.h"
#include "Node.h"
#include "Script.h"
#include "ScriptImpl.h"
#include <GL/gl.h>
#include <iostream>
#include <vector>
#include <chrono>

int main(void)
{
  // Create window with custom configuration
  WindowConfig config;
  config.width = 1024;
  config.height = 768;
  config.title = "OpenGL Scene Loader Demo";
  config.centerOnScreen = true;

  Window window(config);

  if (!window.isValid())
  {
    std::cerr << "Failed to create window" << std::endl;
    return -1;
  }

  // Get available scene files
  auto availableScenes = SceneLoader::getAvailableScenes();
  std::cout << "Available scenes:" << std::endl;
  for (const auto &sceneFile : availableScenes)
  {
    std::cout << "  - " << sceneFile << std::endl;
  }
  std::cout << std::endl;

  // Load the first scene from file
  Scene currentScene = SceneLoader::loadSceneFromFile("scenes/default.scn");
  std::cout << "Loaded scene: " << currentScene.getName() << std::endl;
  std::cout << "Nodes in scene: " << currentScene.getNodeCount() << std::endl;

  // Attach a rotation script to the triangle
  auto rootNode = currentScene.getRoot();
  if (rootNode && rootNode->getChildCount() > 0)
  {
    // Find the triangle and attach a rotation script
    for (auto &child : rootNode->getChildren())
    {
      if (child->getName() == "Triangle1")
      {
        auto rotateScript = std::make_unique<RotateScript>(180.0f); // 180 degrees per second
        child->setScript(std::move(rotateScript));
        std::cout << "Attached rotation script to Triangle1" << std::endl;
        break;
      }
    }
  }

  // Timing variables
  auto lastTime = std::chrono::high_resolution_clock::now();

  // Main render loop
  while (!window.shouldClose())
  {
    // Calculate delta time
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;

    // Update the scene (animations, etc.)
    currentScene.update(deltaTime);

    // Clear the color buffer
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the current scene (all nodes)
    currentScene.render();

    // Swap front and back buffers
    window.swapBuffers();

    // Poll for and process events
    window.pollEvents();
  }

  return 0;
}