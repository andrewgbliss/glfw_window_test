#pragma once

#include "Scene.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Math.h"
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <map>
#include <iostream>

// Scene file parser and loader
class SceneLoader
{
private:
  // Scene file format keywords
  static const std::string SCENE_KEYWORD;
  static const std::string TRIANGLE_KEYWORD;
  static const std::string RECTANGLE_KEYWORD;
  static const std::string POSITION_KEYWORD;
  static const std::string SCALE_KEYWORD;
  static const std::string COLOR_KEYWORD;
  static const std::string NAME_KEYWORD;

public:
  // Load scene from file
  static Scene loadSceneFromFile(const std::string &filename);

  // Save scene to file
  static bool saveSceneToFile(const Scene &scene, const std::string &filename);

  // Get available scene files
  static std::vector<std::string> getAvailableScenes(const std::string &directory = "scenes/");

private:
  // Parse individual object from file
  static std::unique_ptr<Node> parseObject(std::ifstream &file, const std::string &objectType);

  // Parse vector2 from string
  static Vector2 parseVector2(const std::string &str);

  // Parse color from string
  static Color parseColor(const std::string &str);

  // Trim whitespace from string
  static std::string trim(const std::string &str);

  // Split string by delimiter
  static std::vector<std::string> split(const std::string &str, char delimiter);
};

// Scene file format keywords
inline const std::string SceneLoader::SCENE_KEYWORD = "SCENE";
inline const std::string SceneLoader::TRIANGLE_KEYWORD = "TRIANGLE";
inline const std::string SceneLoader::RECTANGLE_KEYWORD = "RECTANGLE";
inline const std::string SceneLoader::POSITION_KEYWORD = "POSITION";
inline const std::string SceneLoader::SCALE_KEYWORD = "SCALE";
inline const std::string SceneLoader::COLOR_KEYWORD = "COLOR";
inline const std::string SceneLoader::NAME_KEYWORD = "NAME";

// Implementation of SceneLoader methods
inline Scene SceneLoader::loadSceneFromFile(const std::string &filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error: Could not open scene file: " << filename << std::endl;
    return Scene("Error Scene");
  }

  Scene scene("Loaded Scene");
  std::string line;
  std::string sceneName = "Loaded Scene";

  while (std::getline(file, line))
  {
    line = trim(line);

    // Skip empty lines and comments
    if (line.empty() || line[0] == '#')
    {
      continue;
    }

    // Parse scene name
    if (line.find(SCENE_KEYWORD) == 0)
    {
      size_t pos = line.find(' ');
      if (pos != std::string::npos)
      {
        sceneName = line.substr(pos + 1);
        scene = Scene(sceneName);
      }
      continue;
    }

    // Parse triangle
    if (line.find(TRIANGLE_KEYWORD) == 0)
    {
      auto triangle = parseObject(file, TRIANGLE_KEYWORD);
      if (triangle)
      {
        scene.addNode(std::move(triangle));
      }
      continue;
    }

    // Parse rectangle
    if (line.find(RECTANGLE_KEYWORD) == 0)
    {
      auto rectangle = parseObject(file, RECTANGLE_KEYWORD);
      if (rectangle)
      {
        scene.addNode(std::move(rectangle));
      }
      continue;
    }
  }

  file.close();
  return scene;
}

inline bool SceneLoader::saveSceneToFile(const Scene &scene, const std::string &filename)
{
  std::ofstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error: Could not create scene file: " << filename << std::endl;
    return false;
  }

  // Write scene header
  file << SCENE_KEYWORD << " " << scene.getName() << std::endl;
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
  scenes.push_back("scenes/default.scn");
  scenes.push_back("scenes/colorful.scn");
  scenes.push_back("scenes/minimal.scn");
  return scenes;
}

inline std::unique_ptr<Node> SceneLoader::parseObject(std::ifstream &file, const std::string &objectType)
{
  std::string name = "Default";
  Position2D position(0, 0);
  Scale2D scale(1, 1);
  Color color(1, 1, 1);

  std::string line;
  while (std::getline(file, line))
  {
    line = trim(line);

    if (line.empty())
    {
      break; // End of object definition
    }

    if (line[0] == '#')
    {
      continue; // Skip comments
    }

    // Parse name
    if (line.find(NAME_KEYWORD) == 0)
    {
      size_t pos = line.find(' ');
      if (pos != std::string::npos)
      {
        name = line.substr(pos + 1);
      }
      continue;
    }

    // Parse position
    if (line.find(POSITION_KEYWORD) == 0)
    {
      size_t pos = line.find(' ');
      if (pos != std::string::npos)
      {
        position = parseVector2(line.substr(pos + 1));
      }
      continue;
    }

    // Parse scale
    if (line.find(SCALE_KEYWORD) == 0)
    {
      size_t pos = line.find(' ');
      if (pos != std::string::npos)
      {
        scale = parseVector2(line.substr(pos + 1));
      }
      continue;
    }

    // Parse color
    if (line.find(COLOR_KEYWORD) == 0)
    {
      size_t pos = line.find(' ');
      if (pos != std::string::npos)
      {
        color = parseColor(line.substr(pos + 1));
      }
      continue;
    }
  }

  // Create the appropriate object
  if (objectType == TRIANGLE_KEYWORD)
  {
    return std::make_unique<Triangle>(name, position, scale, color);
  }
  else if (objectType == RECTANGLE_KEYWORD)
  {
    return std::make_unique<Rectangle>(name, position, scale, color);
  }

  return nullptr;
}

inline Vector2 SceneLoader::parseVector2(const std::string &str)
{
  auto parts = split(str, ',');
  if (parts.size() >= 2)
  {
    return Vector2(std::stof(parts[0]), std::stof(parts[1]));
  }
  return Vector2(0, 0);
}

inline Color SceneLoader::parseColor(const std::string &str)
{
  auto parts = split(str, ',');
  if (parts.size() >= 3)
  {
    return Color(std::stof(parts[0]), std::stof(parts[1]), std::stof(parts[2]));
  }
  return Colors::white;
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