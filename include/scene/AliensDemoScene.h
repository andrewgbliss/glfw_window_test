#pragma once

#include "Scene.h"
#include "../nodes/Sprite2D.h"
#include "../nodes/Animation2D.h"
#include "../core/window/Window.h"
#include "../core/Input.h"
#include <memory>
#include <iostream> // Added for debug output

class AliensDemoScene : public Scene
{
public:
  AliensDemoScene() : Scene("AliensDemo")
  {
    // Create the main alien sprite with all animations
    auto aliensSprite = std::make_unique<Sprite2D>(
        "AliensSprite",
        160.0f, 90.0f, // position (screen coordinates)
        1.0f, 1.0f,    // scale
        "assets/img/sprites/alien-16x16-Sheet.png",
        6, 21 // hframes, vframes
    );

    // Set up all animations for the alien sprite
    if (aliensSprite->getAnimator())
    {
      auto *animator = aliensSprite->getAnimator();

      // IDLE animation
      animator->addAnimation("IDLE", {AnimationFrame(1, 2)}, 8.0f, true);

      // WALK animation
      animator->addAnimation("WALK", {AnimationFrame(3, 6)}, 12.0f, true);

      // RUN animation
      animator->addAnimation("RUN", {AnimationFrame(9, 6)}, 15.0f, true);

      // JUMP animation
      animator->addAnimation("JUMP", {AnimationFrame(15, 6)}, 10.0f, false);

      // PUNCH animation
      animator->addAnimation("PUNCH", {AnimationFrame(21, 2)}, 8.0f, false);

      // KICK animation
      animator->addAnimation("KICK", {AnimationFrame(23, 2)}, 8.0f, false);

      // PUSH animation
      animator->addAnimation("PUSH", {AnimationFrame(25, 6)}, 12.0f, false);

      // SMASH_DOWN animation
      animator->addAnimation("SMASH_DOWN", {AnimationFrame(31, 3)}, 10.0f, false);

      // WALL_CLING animation
      animator->addAnimation("WALL_CLING", {AnimationFrame(34, 2)}, 6.0f, true);

      // LEDGE_GRAB animation
      animator->addAnimation("LEDGE_GRAB", {AnimationFrame(36, 1)}, 6.0f, true);

      // LEDGE_CLIMB animation
      animator->addAnimation("LEDGE_CLIMB", {AnimationFrame(37, 3)}, 8.0f, false);

      // DANGLING animation
      animator->addAnimation("DANGLING", {AnimationFrame(40, 4)}, 8.0f, true);

      // WALKING_SLOPE animation
      animator->addAnimation("WALKING_SLOPE", {AnimationFrame(44, 6)}, 12.0f, true);

      // RUNNING_SLOPE animation
      animator->addAnimation("RUNNING_SLOPE", {AnimationFrame(50, 6)}, 15.0f, true);

      // JUMP_FLIP animation
      animator->addAnimation("JUMP_FLIP", {AnimationFrame(56, 6)}, 10.0f, false);

      // CROUCH_IDLE animation
      animator->addAnimation("CROUCH_IDLE", {AnimationFrame(62, 2)}, 8.0f, true);

      // CROUCH_WALK animation
      animator->addAnimation("CROUCH_WALK", {AnimationFrame(63, 6)}, 12.0f, true);

      // DIE animation
      animator->addAnimation("DIE", {AnimationFrame(70, 6)}, 8.0f, false);

      // SLIDE animation
      animator->addAnimation("SLIDE", {AnimationFrame(76, 2)}, 10.0f, false);

      // SWIM animation
      animator->addAnimation("SWIM", {AnimationFrame(78, 3)}, 8.0f, true);

      // DAMAGE animation
      animator->addAnimation("DAMAGE", {AnimationFrame(81, 2)}, 6.0f, false);

      // LADDER animation
      animator->addAnimation("LADDER", {AnimationFrame(83, 4)}, 8.0f, true);

      // LAND animation
      animator->addAnimation("LAND", {AnimationFrame(87, 3)}, 10.0f, false);

      // Start with IDLE animation
      animator->play("IDLE");
    }

    addNode(std::move(aliensSprite));
  }

  // Override update to handle delayed animation start
  void update(float deltaTime = 0.0f) override
  {
    // Call base class update
    Scene::update(deltaTime);

    // Handle movement and animation based on input
    handleMovement(deltaTime);
  }

  // Override handleInput to manage input handling
  void handleInput() override
  {
    // Call base class input handling
    Scene::handleInput();

    // Update input system
    Input::getInstance().update();
  }

private:
  float startTimer = 0.0f;
  bool animationsStarted = false;
  float moveSpeed = 100.0f; // pixels per second
  bool facingRight = true;

  void handleMovement(float deltaTime)
  {
    auto &input = Input::getInstance();
    auto rootNode = getRoot();

    if (!rootNode)
      return;

    for (auto &child : rootNode->getChildren())
    {
      if (auto sprite = dynamic_cast<Sprite2D *>(child.get()))
      {
        // Get current position
        Position2D currentPos = sprite->getPosition();
        Position2D originalPos = currentPos; // Store original position for comparison
        bool isMoving = false;
        bool isRunning = false;
        std::string targetAnimation = "IDLE";

        // Debug: Check if any movement keys are pressed
        bool leftPressed = input.isActionPressed("move_left");
        bool rightPressed = input.isActionPressed("move_right");
        bool upPressed = input.isActionPressed("move_up");
        bool downPressed = input.isActionPressed("move_down");

        if (leftPressed || rightPressed || upPressed || downPressed)
        {
          std::cout << "Input detected - Left: " << leftPressed
                    << ", Right: " << rightPressed
                    << ", Up: " << upPressed
                    << ", Down: " << downPressed << std::endl;
          std::cout << "Current sprite position: (" << currentPos.x << ", " << currentPos.y << ")" << std::endl;
          std::cout << "Delta time: " << deltaTime << std::endl;
          std::cout << "Move speed: " << moveSpeed << std::endl;
        }

        // Handle horizontal movement
        if (leftPressed)
        {
          currentPos.x -= moveSpeed * deltaTime;
          isMoving = true;
          facingRight = false;
          std::cout << "Moving left - Position change: (" << originalPos.x << " -> " << currentPos.x << ", " << currentPos.y << ")" << std::endl;
        }
        else if (rightPressed)
        {
          currentPos.x += moveSpeed * deltaTime;
          isMoving = true;
          facingRight = true;
          std::cout << "Moving right - Position change: (" << originalPos.x << " -> " << currentPos.x << ", " << currentPos.y << ")" << std::endl;
        }

        // Handle vertical movement
        if (upPressed)
        {
          currentPos.y += moveSpeed * deltaTime;
          isMoving = true;
          std::cout << "Moving up - Position change: (" << originalPos.y << " -> " << currentPos.y << ")" << std::endl;
        }
        else if (downPressed)
        {
          currentPos.y -= moveSpeed * deltaTime;
          isMoving = true;
          std::cout << "Moving down - Position change: (" << originalPos.y << " -> " << currentPos.y << ")" << std::endl;
        }

        // Handle running (hold shift while moving)
        if (input.isKeyPressed(340)) // Left Shift key
        {
          if (isMoving)
          {
            isRunning = true;
            moveSpeed = 200.0f; // Faster speed when running
          }
        }
        else
        {
          moveSpeed = 100.0f; // Normal speed
        }

        // Handle action inputs
        if (input.isActionJustPressed("jump"))
        {
          targetAnimation = "JUMP";
          std::cout << "JUMP action triggered!" << std::endl;
        }
        else if (input.isActionJustPressed("punch"))
        {
          targetAnimation = "PUNCH";
          std::cout << "PUNCH action triggered!" << std::endl;
        }
        else if (input.isActionJustPressed("kick"))
        {
          targetAnimation = "KICK";
          std::cout << "KICK action triggered!" << std::endl;
        }
        else if (input.isActionPressed("crouch"))
        {
          if (isMoving)
            targetAnimation = "CROUCH_WALK";
          else
            targetAnimation = "CROUCH_IDLE";
        }
        else if (isMoving)
        {
          if (isRunning)
            targetAnimation = "RUN";
          else
            targetAnimation = "WALK";
        }
        else
        {
          targetAnimation = "IDLE";
        }

                // Debug: Verify position was actually set
        Position2D newPos = sprite->getPosition();
        if (newPos.x != originalPos.x || newPos.y != originalPos.y)
        {
          std::cout << "Sprite position updated: (" << originalPos.x << ", " << originalPos.y << ") -> (" << newPos.x << ", " << newPos.y << ")" << std::endl;
        }
        else if (leftPressed || rightPressed || upPressed || downPressed)
        {
          std::cout << "WARNING: Position didn't change despite movement input!" << std::endl;
        }

        // Update sprite position
        sprite->setPosition(newPos);

        // Update sprite scale based on facing direction
        Scale2D currentScale = sprite->getScale();
        if (facingRight && currentScale.x < 0)
        {
          sprite->setScale(Scale2D(-currentScale.x, currentScale.y));
        }
        else if (!facingRight && currentScale.x > 0)
        {
          sprite->setScale(Scale2D(-currentScale.x, currentScale.y));
        }

        // Play appropriate animation
        std::string currentAnim = sprite->getCurrentAnimation();
        if (currentAnim != targetAnimation)
        {
          sprite->playAnimation(targetAnimation);
          std::cout << "Animation changed from " << currentAnim << " to " << targetAnimation << std::endl;
        }
      }
    }
  }
};
