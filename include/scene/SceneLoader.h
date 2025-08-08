#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <iostream>

#include "Scene.h"
#include "YamlParser.h"
#include "../nodes/NodeTypeMap.h"
#include "../nodes/Sprite2D.h"
#include "../nodes/Animation2D.h"
#include "../core/Math.h"

// Scene file parser and loader
class SceneLoader
{
public:
  // Load scene from YAML file
  static Scene loadSceneFromYaml(const std::string &filename);

  // Save scene to file
  static bool saveSceneToFile(const Scene &scene, const std::string &filename);

  // Get available scene files
  static std::vector<std::string> getAvailableScenes(const std::string &directory = "scenes/");

private:
  // Parse YAML node
  static std::unique_ptr<Node> parseYamlNode(const YamlParser::YamlNode &nodeData);

  // Parse animations from YAML node
  static void parseAnimations(const YamlParser::YamlNode &animNode, Sprite2D *sprite);

  // Parse vector2 from YAML array
  static Vector2 parseVector2FromArray(const YamlParser::YamlNode &node);

  // Parse color from YAML array
  static Color parseColorFromArray(const YamlParser::YamlNode &node);

  // Trim whitespace from string
  static std::string trim(const std::string &str);

  // Split string by delimiter
  static std::vector<std::string> split(const std::string &str, char delimiter);
};

// Implementation of SceneLoader methods
inline bool SceneLoader::saveSceneToFile(const Scene &scene, const std::string &filename)
{
  std::ofstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error: Could not create scene file: " << filename << std::endl;
    return false;
  }

  // Write scene header
  file << "scene:" << std::endl;
  file << "  name: \"" << scene.getName() << "\"" << std::endl;
  file << std::endl;

  // Write all nodes (this is a simplified version - you'd need to iterate through nodes)
  file << "# Scene saved automatically" << std::endl;
  file << "# Node count: " << scene.getNodeCount() << std::endl;

  file.close();
  return true;
}

inline std::vector<std::string> SceneLoader::getAvailableScenes(const std::string &directory)
{
  std::vector<std::string> scenes;
  // This would need platform-specific directory listing
  // For now, return common scene names
  scenes.push_back("scenes/minimal.yaml");
  scenes.push_back("scenes/colorful.yaml");
  scenes.push_back("scenes/aliens_demo.yaml");
  return scenes;
}

inline std::string SceneLoader::trim(const std::string &str)
{
  size_t start = str.find_first_not_of(" \t\r\n");
  if (start == std::string::npos)
  {
    return "";
  }
  size_t end = str.find_last_not_of(" \t\r\n");
  return str.substr(start, end - start + 1);
}

inline std::vector<std::string> SceneLoader::split(const std::string &str, char delimiter)
{
  std::vector<std::string> tokens;
  std::stringstream ss(str);
  std::string token;

  while (std::getline(ss, token, delimiter))
  {
    tokens.push_back(trim(token));
  }

  return tokens;
}

// YAML Scene Loading Implementation
inline Scene SceneLoader::loadSceneFromYaml(const std::string &filename)
{
  YamlParser::YamlNode root = YamlParser::parseFile(filename);
  std::cout << "Parsed YAML root key: '" << root.key << "'" << std::endl;
  std::cout << "Root children count: " << root.children.size() << std::endl;

  if (root.key.empty())
  {
    std::cerr << "Error: Could not parse YAML file: " << filename << std::endl;
    return Scene("Error Scene");
  }

  Scene scene("Loaded Scene");

  // Parse scene info
  auto sceneIt = root.children.find("scene");
  if (sceneIt != root.children.end())
  {
    auto nameIt = sceneIt->second.children.find("name");
    if (nameIt != sceneIt->second.children.end())
    {
      scene = Scene(nameIt->second.value);
    }
  }

  // Parse nodes
  auto nodesIt = root.children.find("nodes");
  if (nodesIt != root.children.end())
  {
    for (const auto &nodeData : nodesIt->second.array)
    {
      auto node = parseYamlNode(nodeData);
      if (node)
      {
        scene.addNode(std::move(node));
      }
    }
  }

  return scene;
}

inline std::unique_ptr<Node> SceneLoader::parseYamlNode(const YamlParser::YamlNode &nodeData)
{
  auto typeIt = nodeData.children.find("type");
  if (typeIt == nodeData.children.end())
  {
    std::cerr << "Error: Node missing type" << std::endl;
    return nullptr;
  }

  std::string nodeType = typeIt->second.value;
  std::string name = "Default";
  Position2D position(0, 0);
  Scale2D scale(1, 1);
  Color color(1, 1, 1);
  std::string imagePath = "";
  int hframes = 1;
  int vframes = 1;
  int frame = 0;

  // Parse node properties
  auto nameIt = nodeData.children.find("name");
  if (nameIt != nodeData.children.end())
  {
    name = nameIt->second.value;
  }

  auto posIt = nodeData.children.find("position");
  if (posIt != nodeData.children.end())
  {
    std::cout << "Found position in YAML, key: '" << posIt->first << "'" << std::endl;
    std::cout << "Position node value: '" << posIt->second.value << "'" << std::endl;
    std::cout << "Position node array size: " << posIt->second.array.size() << std::endl;
    std::cout << "Position node children size: " << posIt->second.children.size() << std::endl;
    position = parseVector2FromArray(posIt->second);
    std::cout << "Parsed position: (" << position.x << ", " << position.y << ")" << std::endl;
  }
  else
  {
    std::cout << "Position not found in YAML" << std::endl;
  }

  auto scaleIt = nodeData.children.find("scale");
  if (scaleIt != nodeData.children.end())
  {
    std::cout << "Found scale in YAML, key: '" << scaleIt->first << "'" << std::endl;
    std::cout << "Scale node value: '" << scaleIt->second.value << "'" << std::endl;
    std::cout << "Scale node array size: " << scaleIt->second.array.size() << std::endl;
    std::cout << "Scale node children size: " << scaleIt->second.children.size() << std::endl;
    scale = parseVector2FromArray(scaleIt->second);
    std::cout << "Parsed scale: (" << scale.x << ", " << scale.y << ")" << std::endl;
  }
  else
  {
    std::cout << "Scale not found in YAML" << std::endl;
  }

  auto colorIt = nodeData.children.find("color");
  if (colorIt != nodeData.children.end())
  {
    color = parseColorFromArray(colorIt->second);
  }

  auto imageIt = nodeData.children.find("image_path");
  if (imageIt != nodeData.children.end())
  {
    imagePath = imageIt->second.value;
  }

  auto hframesIt = nodeData.children.find("hframes");
  if (hframesIt != nodeData.children.end())
  {
    hframes = std::stoi(hframesIt->second.value);
  }

  auto vframesIt = nodeData.children.find("vframes");
  if (vframesIt != nodeData.children.end())
  {
    vframes = std::stoi(vframesIt->second.value);
  }

  auto frameIt = nodeData.children.find("frame");
  if (frameIt != nodeData.children.end())
  {
    frame = std::stoi(frameIt->second.value);
  }

  // Create the node
  auto nodeFactory = NodeTypeMap::getNodeFactory(nodeType);
  if (nodeFactory)
  {
    auto node = nodeFactory(name, position, scale, color);

    // Special handling for Sprite2D nodes
    if (nodeType == "Sprite2D")
    {
      auto sprite2D = dynamic_cast<Sprite2D *>(node.get());
      if (sprite2D)
      {
        std::cout << "Creating Sprite2D: " << name << std::endl;
        if (!imagePath.empty())
        {
          std::cout << "Loading texture: " << imagePath << std::endl;
          sprite2D->loadTexture(imagePath);
        }
        sprite2D->setHFrames(hframes);
        sprite2D->setVFrames(vframes);
        sprite2D->setFrame(frame);
        std::cout << "Set frames: " << hframes << "x" << vframes << ", current frame: " << frame << std::endl;

        // Parse animations
        auto animIt = nodeData.children.find("animations");
        if (animIt != nodeData.children.end())
        {
          std::cout << "Parsing animations..." << std::endl;
          parseAnimations(animIt->second, sprite2D);
        }
        else
        {
          std::cout << "No animations found in YAML" << std::endl;
        }
      }
    }

    return node;
  }

  std::cerr << "Warning: Unknown node type '" << nodeType << "'" << std::endl;
  return nullptr;
}

inline void SceneLoader::parseAnimations(const YamlParser::YamlNode &animNode, Sprite2D *sprite)
{
  if (!sprite || !sprite->getAnimator())
  {
    return;
  }

  Animation2D *animator = sprite->getAnimator();

  for (const auto &animPair : animNode.children)
  {
    const std::string &animName = animPair.first;
    const YamlParser::YamlNode &animData = animPair.second;

    // Parse animation data
    int startFrame = 0;
    int frameCount = 1;
    float frameRate = 12.0f;
    bool loop = true;

    auto startIt = animData.children.find("start_frame");
    if (startIt != animData.children.end())
    {
      startFrame = std::stoi(startIt->second.value);
    }

    auto countIt = animData.children.find("frame_count");
    if (countIt != animData.children.end())
    {
      frameCount = std::stoi(countIt->second.value);
    }

    auto rateIt = animData.children.find("frame_rate");
    if (rateIt != animData.children.end())
    {
      frameRate = std::stof(rateIt->second.value);
    }

    auto loopIt = animData.children.find("loop");
    if (loopIt != animData.children.end())
    {
      loop = (loopIt->second.value == "true");
    }

    // Create animation frame
    std::vector<AnimationFrame> frames;
    frames.push_back(AnimationFrame(startFrame, frameCount));

    // Add animation to animator
    animator->addAnimation(animName, frames, frameRate, loop);

    std::cout << "Added animation: " << animName << " (start: " << startFrame
              << ", count: " << frameCount << ", rate: " << frameRate
              << ", loop: " << (loop ? "true" : "false") << ")" << std::endl;
  }
}

inline Vector2 SceneLoader::parseVector2FromArray(const YamlParser::YamlNode &node)
{
  std::cout << "parseVector2FromArray called, array size: " << node.array.size() << std::endl;
  if (node.array.size() >= 2)
  {
    float x = std::stof(node.array[0].value);
    float y = std::stof(node.array[1].value);
    std::cout << "Parsed values: x=" << x << ", y=" << y << std::endl;
    return Vector2(x, y);
  }
  std::cout << "Array too small, returning (0,0)" << std::endl;
  return Vector2(0, 0);
}

inline Color SceneLoader::parseColorFromArray(const YamlParser::YamlNode &node)
{
  if (node.array.size() >= 3)
  {
    float r = std::stof(node.array[0].value);
    float g = std::stof(node.array[1].value);
    float b = std::stof(node.array[2].value);
    return Color(r, g, b);
  }
  return Colors::white;
}