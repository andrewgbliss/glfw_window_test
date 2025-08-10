#pragma once

#include "Scene.h"
#include "../nodes/Rectangle.h"
#include "../core/Input.h"

class MinimalScene : public Scene
{
private:
  float moveSpeed;

public:
  MinimalScene() : Scene("MinimalScene"), moveSpeed(200.0f)
  {
    // Create a simple red rectangle
    auto rect = std::make_unique<Rectangle>(
        "TestRectangle",
        0.0f, 0.0f,       // position
        1.0f, 1.0f,       // scale
        1.0f, 0.0f, 0.0f, // color (red)
        50.0f, 50.0f      // width, height (50x50 pixels)
    );

    addNode(std::move(rect));
  }

  // Handle input for scene
  void handleInput() override
  {
    std::cout << "MinimalScene::handleInput() called" << std::endl;

    // Find the rectangle by name
    Rectangle *testRectangle = nullptr;
    auto rootNode = getRoot();
    if (rootNode)
    {
      for (auto &child : rootNode->getChildren())
      {
        if (child->getName() == "TestRectangle")
        {
          testRectangle = dynamic_cast<Rectangle *>(child.get());
          break;
        }
      }
    }

    if (!testRectangle)
    {
      std::cout << "TestRectangle not found!" << std::endl;
      return;
    }

    auto &input = Input::getInstance();

    float deltaX = 0.0f;
    float deltaY = 0.0f;

    if (input.isActionPressed("move_left"))
    {
      deltaX -= 1.0f;
      std::cout << "Moving left" << std::endl;
    }
    if (input.isActionPressed("move_right"))
    {
      deltaX += 1.0f;
      std::cout << "Moving right" << std::endl;
    }
    if (input.isActionPressed("move_up"))
    {
      deltaY -= 1.0f;
      std::cout << "Moving up" << std::endl;
    }
    if (input.isActionPressed("move_down"))
    {
      deltaY += 1.0f;
      std::cout << "Moving down" << std::endl;
    }

    // Apply movement
    if (deltaX != 0.0f || deltaY != 0.0f)
    {
      Position2D currentPos = testRectangle->getPosition();
      Position2D newPos = Position2D(currentPos.x + deltaX, currentPos.y + deltaY);
      testRectangle->setPosition(newPos);
      std::cout << "Rectangle moved from (" << currentPos.x << ", " << currentPos.y
                << ") to (" << newPos.x << ", " << newPos.y << ")" << std::endl;
    }
  }
};
