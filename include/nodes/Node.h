#pragma once

#include "../core/Math.h"
#include <vector>
#include <memory>
#include <string>

// Base Node class - purely abstract, no transform properties
class Node
{
protected:
  std::string name;
  std::vector<std::unique_ptr<Node>> children;
  Node *parent;

public:
  Node(const std::string &nodeName = "Node")
      : name(nodeName), parent(nullptr) {}

  virtual ~Node() = default;

  // Name management
  const std::string &getName() const { return name; }
  void setName(const std::string &nodeName) { name = nodeName; }

  // Parent-child relationship
  Node *getParent() const { return parent; }
  void setParent(Node *newParent) { parent = newParent; }

  void addChild(std::unique_ptr<Node> child);
  std::unique_ptr<Node> removeChild(Node *child);
  void removeAllChildren() { children.clear(); }

  const std::vector<std::unique_ptr<Node>> &getChildren() const { return children; }
  size_t getChildCount() const { return children.size(); }

  // Virtual methods for rendering and updating
  virtual void render() const = 0;
  virtual void update(float deltaTime = 0.0f);

  // Virtual method for input handling
  virtual void handleInput();

  // Scene graph traversal
  virtual void renderRecursive() const;
  virtual void updateRecursive(float deltaTime = 0.0f);
  virtual void handleInputRecursive();
};

// Concrete Root Node class that can be instantiated
class RootNode : public Node
{
public:
  RootNode(const std::string &nodeName = "Root") : Node(nodeName) {}

  void render() const override
  {
    // Root node doesn't render anything, just manages children
  }
};

// Implementation of Node methods
inline void Node::addChild(std::unique_ptr<Node> child)
{
  if (child)
  {
    child->setParent(this);
    children.push_back(std::move(child));
  }
}

inline std::unique_ptr<Node> Node::removeChild(Node *child)
{
  for (auto it = children.begin(); it != children.end(); ++it)
  {
    if (it->get() == child)
    {
      std::unique_ptr<Node> removed = std::move(*it);
      children.erase(it);
      removed->setParent(nullptr);
      return removed;
    }
  }
  return nullptr;
}

inline void Node::update(float deltaTime)
{
  // Base node update - can be overridden by derived classes
}

inline void Node::handleInput()
{
  // Base node input handling - can be overridden by derived classes
}

inline void Node::renderRecursive() const
{
  // Render this node
  render();

  // Render all children
  for (const auto &child : children)
  {
    child->renderRecursive();
  }
}

inline void Node::updateRecursive(float deltaTime)
{
  // Update this node
  update(deltaTime);

  // Update all children
  for (auto &child : children)
  {
    child->updateRecursive(deltaTime);
  }
}

inline void Node::handleInputRecursive()
{
  // Handle input for this node
  handleInput();

  // Handle input for all children
  for (auto &child : children)
  {
    child->handleInputRecursive();
  }
}