#pragma once

#include "entities/Triangle.h"
#include "entities/Rectangle.h"
#include "scripts/Scripts.h"
#include "Math.h"
#include <memory>
#include <map>
#include <functional>
#include <string>
#include <vector>

// Node type mapping system
class NodeTypeMap
{
public:
  // Node factory function type
  using NodeFactory = std::function<std::unique_ptr<Node>(const std::string &, const Position2D &, const Scale2D &, const Color &)>;

  // Script factory function type
  using ScriptFactory = std::function<std::unique_ptr<Node>(const std::string &, const Position2D &, const Scale2D &, const Color &, const std::vector<std::string> &)>;

  // Get node factory by type name
  static NodeFactory getNodeFactory(const std::string &nodeType);

  // Get script factory by type name
  static ScriptFactory getScriptFactory(const std::string &scriptType);

  // Check if node type exists
  static bool hasNodeType(const std::string &nodeType);

  // Check if script type exists
  static bool hasScriptType(const std::string &scriptType);

private:
  // Node type registry
  static std::map<std::string, NodeFactory> nodeTypes;

  // Script type registry
  static std::map<std::string, ScriptFactory> scriptTypes;

  // Initialize the maps
  static void initializeMaps();

  // Helper function to trim strings
  static std::string trim(const std::string &str);
};

// Initialize the maps
inline std::map<std::string, NodeTypeMap::NodeFactory> NodeTypeMap::nodeTypes;
inline std::map<std::string, NodeTypeMap::ScriptFactory> NodeTypeMap::scriptTypes;

inline NodeTypeMap::NodeFactory NodeTypeMap::getNodeFactory(const std::string &nodeType)
{
  // Initialize maps on first access
  static bool initialized = false;
  if (!initialized)
  {
    initializeMaps();
    initialized = true;
  }

  auto it = nodeTypes.find(nodeType);
  if (it != nodeTypes.end())
  {
    return it->second;
  }

  // Return nullptr if not found
  return nullptr;
}

inline NodeTypeMap::ScriptFactory NodeTypeMap::getScriptFactory(const std::string &scriptType)
{
  // Initialize maps on first access
  static bool initialized = false;
  if (!initialized)
  {
    initializeMaps();
    initialized = true;
  }

  auto it = scriptTypes.find(scriptType);
  if (it != scriptTypes.end())
  {
    return it->second;
  }

  // Return nullptr if not found
  return nullptr;
}

inline bool NodeTypeMap::hasNodeType(const std::string &nodeType)
{
  return getNodeFactory(nodeType) != nullptr;
}

inline bool NodeTypeMap::hasScriptType(const std::string &scriptType)
{
  return getScriptFactory(scriptType) != nullptr;
}

inline void NodeTypeMap::initializeMaps()
{
  // Register base node types using class names
  nodeTypes["Triangle"] = [](const std::string &name, const Position2D &position, const Scale2D &scale, const Color &color)
  {
    return std::make_unique<Triangle>(name, position, scale, color);
  };

  nodeTypes["Rectangle"] = [](const std::string &name, const Position2D &position, const Scale2D &scale, const Color &color)
  {
    return std::make_unique<Rectangle>(name, position, scale, color);
  };

  // Register script types using class names
  scriptTypes["RotatingTriangle"] = [](const std::string &name, const Position2D &position, const Scale2D &scale, const Color &color, const std::vector<std::string> &params)
  {
    float speed = 90.0f; // default
    if (!params.empty())
    {
      try
      {
        speed = std::stof(trim(params[0]));
      }
      catch (...)
      {
      }
    }
    return std::make_unique<RotatingTriangle>(name, position, scale, color, speed);
  };

  scriptTypes["OscillatingRectangle"] = [](const std::string &name, const Position2D &position, const Scale2D &scale, const Color &color, const std::vector<std::string> &params)
  {
    float amplitude = 0.1f; // default
    float frequency = 2.0f; // default
    if (params.size() >= 1)
    {
      try
      {
        amplitude = std::stof(trim(params[0]));
      }
      catch (...)
      {
      }
    }
    if (params.size() >= 2)
    {
      try
      {
        frequency = std::stof(trim(params[1]));
      }
      catch (...)
      {
      }
    }
    return std::make_unique<OscillatingRectangle>(name, position, scale, color, amplitude, frequency);
  };

  scriptTypes["PulsingTriangle"] = [](const std::string &name, const Position2D &position, const Scale2D &scale, const Color &color, const std::vector<std::string> &params)
  {
    float baseScale = 1.0f;   // default
    float pulseAmount = 0.2f; // default
    float frequency = 3.0f;   // default
    if (params.size() >= 1)
    {
      try
      {
        baseScale = std::stof(trim(params[0]));
      }
      catch (...)
      {
      }
    }
    if (params.size() >= 2)
    {
      try
      {
        pulseAmount = std::stof(trim(params[1]));
      }
      catch (...)
      {
      }
    }
    if (params.size() >= 3)
    {
      try
      {
        frequency = std::stof(trim(params[2]));
      }
      catch (...)
      {
      }
    }
    return std::make_unique<PulsingTriangle>(name, position, scale, color, baseScale, pulseAmount, frequency);
  };
}

inline std::string NodeTypeMap::trim(const std::string &str)
{
  size_t start = str.find_first_not_of(" \t\r\n");
  if (start == std::string::npos)
  {
    return "";
  }
  size_t end = str.find_last_not_of(" \t\r\n");
  return str.substr(start, end - start + 1);
}