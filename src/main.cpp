#include "core/Window.h"
#include "entities/Triangle.h"
#include "entities/Rectangle.h"
#include "core/Scene.h"
#include "core/SceneLoader.h"
#include "core/Node.h"
#include "scripts/Scripts.h"
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

  // Load the inheritance demo scene from file
  Scene currentScene = SceneLoader::loadSceneFromFile("scenes/inheritance_demo.scn");
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
    }
  }
  std::cout << std::endl;

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