#pragma once

#include "Scene.h"

class SimpleScene : public Scene
{
public:
  SimpleScene() : Scene("SimpleScene")
  {
    // Simple scene with no additional nodes
    // Just the root node
  }
};
