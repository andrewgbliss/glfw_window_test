#pragma once

#include "Node.h"
#include "Triangle.h"
#include "Rectangle.h"
#include <memory>
#include <string>

// Scene class to manage nodes
class Scene
{
private:
  std::string name;
  std::unique_ptr<RootNode> rootNode;

public:
  Scene(const std::string &sceneName = "Default Scene")
      : name(sceneName), rootNode(std::make_unique<RootNode>("Root")) {}

  // Add nodes to the scene
  void addNode(std::unique_ptr<Node> node);

  // Remove nodes (by name)
  std::unique_ptr<Node> removeNode(const std::string &nodeName);

  // Clear all nodes
  void clear() { rootNode->removeAllChildren(); }

  // Get scene info
  const std::string &getName() const { return name; }
  size_t getNodeCount() const { return rootNode->getChildCount(); }

  // Get root node
  RootNode *getRoot() { return rootNode.get(); }
  const RootNode *getRoot() const { return rootNode.get(); }

  // Render all nodes in the scene
  void render() const;

  // Update all nodes in the scene
  void update(float deltaTime = 0.0f);

  // Create predefined scenes
  static Scene createDefaultScene();
  static Scene createColorfulScene();
  static Scene createMinimalScene();
};

// Implementation of Scene methods
inline void Scene::addNode(std::unique_ptr<Node> node)
{
  if (node)
  {
    rootNode->addChild(std::move(node));
  }
}

inline std::unique_ptr<Node> Scene::removeNode(const std::string &nodeName)
{
  // Search through children to find node with matching name
  for (auto &child : rootNode->getChildren())
  {
    if (child->getName() == nodeName)
    {
      return rootNode->removeChild(child.get());
    }
  }
  return nullptr;
}

inline void Scene::render() const
{
  rootNode->renderRecursive();
}

inline void Scene::update(float deltaTime)
{
  rootNode->updateRecursive(deltaTime);
}

// Predefined scene creation methods
inline Scene Scene::createDefaultScene()
{
  Scene scene("Default Scene");

  // Add triangle
  auto triangle = std::make_unique<Triangle>("Triangle1",
                                             Position2D(-0.3f, 0.0f),
                                             Scale2D(0.5f, 0.5f),
                                             Colors::orange);
  scene.addNode(std::move(triangle));

  // Add rectangles
  auto rect1 = std::make_unique<Rectangle>("Rectangle1",
                                           Position2D(-0.6f, -0.3f),
                                           Scale2D(0.4f, 0.3f),
                                           Colors::red);
  scene.addNode(std::move(rect1));

  auto rect2 = std::make_unique<Rectangle>("Rectangle2",
                                           Position2D(0.2f, 0.2f),
                                           Scale2D(0.3f, 0.4f),
                                           Colors::green);
  scene.addNode(std::move(rect2));

  auto rect3 = std::make_unique<Rectangle>("Rectangle3",
                                           Position2D(0.0f, -0.5f),
                                           Scale2D(0.5f, 0.2f),
                                           Colors::blue);
  scene.addNode(std::move(rect3));

  return scene;
}

inline Scene Scene::createColorfulScene()
{
  Scene scene("Colorful Scene");

  // Add multiple triangles with different colors
  auto triangle1 = std::make_unique<Triangle>("Triangle1",
                                              Position2D(-0.4f, 0.2f),
                                              Scale2D(0.3f, 0.3f),
                                              Colors::purple);
  scene.addNode(std::move(triangle1));

  auto triangle2 = std::make_unique<Triangle>("Triangle2",
                                              Position2D(0.4f, -0.2f),
                                              Scale2D(0.3f, 0.3f),
                                              Colors::cyan);
  scene.addNode(std::move(triangle2));

  // Add multiple rectangles with different colors
  auto rect1 = std::make_unique<Rectangle>("Rectangle1",
                                           Position2D(-0.7f, 0.4f),
                                           Scale2D(0.2f, 0.2f),
                                           Colors::yellow);
  scene.addNode(std::move(rect1));

  auto rect2 = std::make_unique<Rectangle>("Rectangle2",
                                           Position2D(0.7f, -0.4f),
                                           Scale2D(0.2f, 0.2f),
                                           Colors::magenta);
  scene.addNode(std::move(rect2));

  auto rect3 = std::make_unique<Rectangle>("Rectangle3",
                                           Position2D(0.0f, 0.6f),
                                           Scale2D(0.3f, 0.2f),
                                           Colors::lime);
  scene.addNode(std::move(rect3));

  auto rect4 = std::make_unique<Rectangle>("Rectangle4",
                                           Position2D(0.0f, -0.6f),
                                           Scale2D(0.3f, 0.2f),
                                           Colors::pink);
  scene.addNode(std::move(rect4));

  return scene;
}

inline Scene Scene::createMinimalScene()
{
  Scene scene("Minimal Scene");

  // Add just one triangle and one rectangle
  auto triangle = std::make_unique<Triangle>("Triangle1",
                                             Position2D(0.0f, 0.0f),
                                             Scale2D(0.4f, 0.4f),
                                             Colors::gold);
  scene.addNode(std::move(triangle));

  auto rect = std::make_unique<Rectangle>("Rectangle1",
                                          Position2D(0.0f, 0.0f),
                                          Scale2D(0.6f, 0.6f),
                                          Colors::white);
  scene.addNode(std::move(rect));

  return scene;
}