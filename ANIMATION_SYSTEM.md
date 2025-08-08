# Animation System Documentation

## Overview

The animation system allows you to create and manage sprite sheet animations for Sprite2D nodes. It supports multiple named animations with configurable frame rates, looping, and frame counts.

## Components

### Animation2D Class

The `Animation2D` class manages animations for a sprite:

- **Animation Management**: Add, remove, and manage multiple animations
- **Playback Control**: Play, stop, pause, and resume animations
- **Frame Timing**: Automatic frame progression based on frame rate
- **Looping**: Configurable looping behavior for each animation

### Animation Structure

Each animation contains:

- **Name**: Unique identifier for the animation
- **Frames**: Vector of AnimationFrame objects
- **Frame Rate**: Frames per second (FPS)
- **Loop**: Whether the animation should loop

### AnimationFrame Structure

Each frame contains:

- **startFrame**: The starting frame index in the sprite sheet
- **frameCount**: Number of frames to play in sequence

## YAML Scene Format

Scenes can now be defined in YAML format with animation support:

```yaml
scene:
  name: "MyScene"
  description: "A scene with animations"

nodes:
  - type: "Sprite2D"
    name: "AnimatedSprite"
    position: [0.0, 0.0]
    scale: [1.0, 1.0]
    color: [1.0, 1.0, 1.0]
    image_path: "path/to/spritesheet.png"
    hframes: 6
    vframes: 21
    frame: 0
    animations:
      IDLE:
        start_frame: 1
        frame_count: 2
        frame_rate: 8.0
        loop: true
      WALK:
        start_frame: 3
        frame_count: 6
        frame_rate: 12.0
        loop: true
      JUMP:
        start_frame: 15
        frame_count: 6
        frame_rate: 10.0
        loop: false
```

## Usage

### Loading Animations from YAML

Animations are automatically loaded when parsing YAML scene files:

```cpp
Scene scene = SceneLoader::loadSceneFromYaml("scenes/my_scene.yaml");
```

### Playing Animations

```cpp
// Get a sprite node
auto sprite = dynamic_cast<Sprite2D*>(node.get());

// Play an animation
sprite->playAnimation("WALK");

// Update animations in the game loop
sprite->updateAnimation(deltaTime);
```

### Animation Control

```cpp
// Stop current animation
sprite->stopAnimation();

// Get current animation name
std::string currentAnim = sprite->getCurrentAnimation();

// Check if animation is playing
bool isPlaying = sprite->getAnimator()->isPlaying();
```

## Example Scene Files

- `scenes/aliens_demo.yaml`: Complete alien sprite with all animations
- `scenes/colorful.yaml`: Multiple sprites with different animations
- `scenes/minimal.yaml`: Basic scene without animations

## Keyboard Controls

In the demo application:

- **Space**: Cycle through different animations (IDLE → WALK → RUN → JUMP → PUNCH → KICK → IDLE)

## Implementation Details

### Animation Frame Calculation

The system calculates the current frame index based on:

1. Current animation's frame rate
2. Elapsed time since animation start
3. Frame count and start frame from the animation data

### Sprite Sheet Support

The system works with sprite sheets that have:

- Horizontal frames (hframes)
- Vertical frames (vframes)
- Frame indexing starting from 0

### Performance

- Animations are updated efficiently using delta time
- Frame calculations are done only when needed
- Memory usage is minimal with shared animation data structures
