#pragma once

#include "Scene.h"
#include "../nodes/Rectangle.h"

class TestScene : public Scene
{
public:
  TestScene() : Scene("TestScene")
  {
    // Create a test rectangle
    auto testRect = std::make_unique<Rectangle>(
        "TestRect",
        0.0f, 0.0f,      // position
        1.0f, 1.0f,      // scale
        1.0f, 0.0f, 0.0f // color (red)
    );

    addNode(std::move(testRect));
  }
};
