#pragma once

#include "Scene.h"
#include "../nodes/Sprite2D.h"
#include "../nodes/Triangle.h"
#include "../nodes/Animation2D.h"

class ColorfulScene : public Scene
{
public:
  ColorfulScene() : Scene("ColorfulScene")
  {
    // Create first alien sprite with IDLE and WALK animations
    auto alien1 = std::make_unique<Sprite2D>(
        "Alien1",
        -2.0f, 0.0f, // position
        1.0f, 1.0f,  // scale
        "assets/img/sprites/alien-16x16-Sheet.png",
        6, 21 // hframes, vframes
    );

    // Set up animations for alien1
    if (alien1->getAnimator())
    {
      // IDLE animation
      alien1->getAnimator()->addAnimation("IDLE",
                                          {AnimationFrame(1, 2)}, 8.0f, true);

      // WALK animation
      alien1->getAnimator()->addAnimation("WALK",
                                          {AnimationFrame(3, 6)}, 12.0f, true);

      // Start with IDLE animation
      alien1->getAnimator()->play("IDLE");
    }

    addNode(std::move(alien1));

    // Create second alien sprite with RUN and JUMP animations
    auto alien2 = std::make_unique<Sprite2D>(
        "Alien2",
        2.0f, 0.0f, // position
        1.0f, 1.0f, // scale
        "assets/img/sprites/alien-16x16-Sheet.png",
        6, 21 // hframes, vframes
    );

    // Set up animations for alien2
    if (alien2->getAnimator())
    {
      // RUN animation
      alien2->getAnimator()->addAnimation("RUN",
                                          {AnimationFrame(9, 6)}, 15.0f, true);

      // JUMP animation
      alien2->getAnimator()->addAnimation("JUMP",
                                          {AnimationFrame(15, 6)}, 10.0f, false);

      // Start with RUN animation
      alien2->getAnimator()->play("RUN");
    }

    addNode(std::move(alien2));

    // Create background triangle
    auto backgroundTriangle = std::make_unique<Triangle>(
        "BackgroundTriangle",
        0.0f, -2.0f,     // position
        0.5f, 0.5f,      // scale
        0.0f, 1.0f, 0.0f // color (green)
    );

    addNode(std::move(backgroundTriangle));
  }
};
