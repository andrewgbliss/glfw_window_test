#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

// Simple YAML parser for scene files
class YamlParser
{
public:
  struct YamlNode
  {
    std::string key;
    std::string value;
    std::map<std::string, YamlNode> children;
    std::vector<YamlNode> array;
    bool isArray = false;
    int indentLevel = 0;
  };

  static YamlNode parseFile(const std::string &filename);
  static YamlNode parseString(const std::string &content);

private:
  static YamlNode parseNode(std::vector<std::string> &lines, size_t &index, int currentIndent);
  static int getIndentLevel(const std::string &line);
  static std::string trim(const std::string &str);
  static std::vector<std::string> split(const std::string &str, char delimiter);
  static bool isArrayItem(const std::string &line);
  static std::string extractKey(const std::string &line);
  static std::string extractValue(const std::string &line);
};
