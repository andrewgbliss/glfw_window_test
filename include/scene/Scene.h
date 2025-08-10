#pragma once

#include "../nodes/Node.h"
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

  virtual ~Scene() = default;

  // Move constructor
  Scene(Scene &&other) noexcept
      : name(std::move(other.name)), rootNode(std::move(other.rootNode)) {}

  // Move assignment operator
  Scene &operator=(Scene &&other) noexcept
  {
    if (this != &other)
    {
      name = std::move(other.name);
      rootNode = std::move(other.rootNode);
    }
    return *this;
  }

  // Delete copy constructor and assignment operator
  Scene(const Scene &) = delete;
  Scene &operator=(const Scene &) = delete;

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
  virtual void update(float deltaTime = 0.0f);

  // Handle scene-specific input (can be overridden by derived scenes)
  virtual void handleInput();

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

inline void Scene::handleInput()
{
  // Base scene input handling - can be overridden by derived scenes
  // This is where scene-specific input logic would go
}
