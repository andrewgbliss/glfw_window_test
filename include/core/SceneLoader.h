#pragma once

#include "Scene.h"
#include "NodeTypeMap.h"
#include "Math.h"
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <iostream>

// Scene file parser and loader
class SceneLoader
{
private:
  // Scene file format keywords
  static const std::string SCENE_KEYWORD;
  static const std::string POSITION_KEYWORD;
  static const std::string SCALE_KEYWORD;
  static const std::string COLOR_KEYWORD;
  static const std::string NAME_KEYWORD;
  static const std::string SCRIPT_KEYWORD;
  static const std::string SCRIPT_PARAMS_KEYWORD;

public:
  // Load scene from file
  static Scene loadSceneFromFile(const std::string &filename);

  // Save scene to file
  static bool saveSceneToFile(const Scene &scene, const std::string &filename);

  // Get available scene files
  static std::vector<std::string> getAvailableScenes(const std::string &directory = "scenes/");

private:
  // Parse individual object from file
  static std::unique_ptr<Node> parseObject(std::ifstream &file, const std::string &nodeType);

  // Parse script from string
  static std::unique_ptr<Node> parseScript(const std::string &scriptType, const std::string &params,
                                           const std::string &name, const Position2D &position,
                                           const Scale2D &scale, const Color &color);

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
inline const std::string SceneLoader::POSITION_KEYWORD = "POSITION";
inline const std::string SceneLoader::SCALE_KEYWORD = "SCALE";
inline const std::string SceneLoader::COLOR_KEYWORD = "COLOR";
inline const std::string SceneLoader::NAME_KEYWORD = "NAME";
inline const std::string SceneLoader::SCRIPT_KEYWORD = "SCRIPT";
inline const std::string SceneLoader::SCRIPT_PARAMS_KEYWORD = "SCRIPT_PARAMS";

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

    // Parse any node type dynamically
    // Check if this line is a node type (not a keyword)
    bool isNodeType = true;
    if (line == SCENE_KEYWORD || line == POSITION_KEYWORD || line == SCALE_KEYWORD ||
        line == COLOR_KEYWORD || line == NAME_KEYWORD || line == SCRIPT_KEYWORD ||
        line == SCRIPT_PARAMS_KEYWORD)
    {
      isNodeType = false;
    }

    if (isNodeType)
    {
      auto node = parseObject(file, line);
      if (node)
      {
        scene.addNode(std::move(node));
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
  scenes.push_back("scenes/inheritance_demo.scn");
  return scenes;
}

inline std::unique_ptr<Node> SceneLoader::parseObject(std::ifstream &file, const std::string &nodeType)
{
  std::string name = "Default";
  Position2D position(0, 0);
  Scale2D scale(1, 1);
  Color color(1, 1, 1);
  std::string scriptType = "";
  std::string scriptParams = "";

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

    // Parse script
    if (line.find(SCRIPT_KEYWORD) == 0)
    {
      size_t pos = line.find(' ');
      if (pos != std::string::npos)
      {
        scriptType = line.substr(pos + 1);

        // Look for script parameters on the next line
        std::streampos currentPos = file.tellg();
        if (std::getline(file, line))
        {
          line = trim(line);
          if (line.find(SCRIPT_PARAMS_KEYWORD) == 0)
          {
            size_t paramPos = line.find(' ');
            if (paramPos != std::string::npos)
            {
              scriptParams = line.substr(paramPos + 1);
            }
          }
          else
          {
            // If no parameters line, go back
            file.seekg(currentPos);
          }
        }
      }
      continue;
    }
  }

  // If a script was specified, create the script-based node
  if (!scriptType.empty())
  {
    auto paramParts = split(scriptParams, ',');
    return parseScript(scriptType, scriptParams, name, position, scale, color);
  }

  // Look up the node type in the map
  auto nodeFactory = NodeTypeMap::getNodeFactory(nodeType);
  if (nodeFactory)
  {
    return nodeFactory(name, position, scale, color);
  }

  // If node type not found, return nullptr
  std::cerr << "Warning: Unknown node type '" << nodeType << "'" << std::endl;
  return nullptr;
}

inline std::unique_ptr<Node> SceneLoader::parseScript(const std::string &scriptType, const std::string &params,
                                                      const std::string &name, const Position2D &position,
                                                      const Scale2D &scale, const Color &color)
{
  auto paramParts = split(params, ',');

  // Look up the script type in the map
  auto scriptFactory = NodeTypeMap::getScriptFactory(scriptType);
  if (scriptFactory)
  {
    return scriptFactory(name, position, scale, color, paramParts);
  }

  // If script type not found, return nullptr
  std::cerr << "Warning: Unknown script type '" << scriptType << "'" << std::endl;
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