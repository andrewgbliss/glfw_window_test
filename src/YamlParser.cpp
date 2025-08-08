#include "../include/scene/YamlParser.h"
#include <algorithm>

YamlParser::YamlNode YamlParser::parseFile(const std::string &filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error: Could not open YAML file: " << filename << std::endl;
    return YamlNode();
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line))
  {
    lines.push_back(line);
  }
  file.close();

  // Find the first non-comment line
  size_t startIndex = 0;
  for (size_t i = 0; i < lines.size(); i++)
  {
    std::string trimmedLine = trim(lines[i]);
    if (!trimmedLine.empty() && trimmedLine[0] != '#')
    {
      startIndex = i;
      break;
    }
  }

  size_t index = startIndex;
  YamlNode root = parseNode(lines, index, 0);

  return root;
}

YamlParser::YamlNode YamlParser::parseString(const std::string &content)
{
  std::vector<std::string> lines;
  std::stringstream ss(content);
  std::string line;
  while (std::getline(ss, line))
  {
    lines.push_back(line);
  }

  size_t index = 0;
  return parseNode(lines, index, 0);
}

YamlParser::YamlNode YamlParser::parseNode(std::vector<std::string> &lines, size_t &index, int currentIndent)
{
  YamlNode node;

  while (index < lines.size())
  {
    std::string line = lines[index];
    std::string trimmedLine = trim(line);

    // Skip empty lines and comments
    if (trimmedLine.empty() || trimmedLine[0] == '#')
    {
      index++;
      continue;
    }

    int indentLevel = getIndentLevel(line);

    // If we've gone deeper than our current level, we're done with this node
    if (indentLevel < currentIndent)
    {
      break;
    }

    // If this is at our current level, it's a sibling
    if (indentLevel == currentIndent)
    {
      if (isArrayItem(trimmedLine))
      {
        // This is an array item
        YamlNode arrayItem;
        arrayItem.key = "item";
        arrayItem.indentLevel = indentLevel;

        // Extract the key-value pair from the array item line
        std::string arrayItemContent = trimmedLine.substr(2); // Remove the "- " prefix
        std::string key = extractKey(arrayItemContent);
        std::string value = extractValue(arrayItemContent);

        if (!key.empty())
        {
          YamlNode valueNode;
          valueNode.key = key;
          valueNode.value = value;
          arrayItem.children[key] = valueNode;
        }

        // Parse the array item content (indented children)
        index++;
        if (index < lines.size())
        {
          int nextIndent = getIndentLevel(lines[index]);
          if (nextIndent > indentLevel)
          {
            YamlNode childNode = parseNode(lines, index, nextIndent);
            // Merge the child node's children into the array item
            for (const auto &child : childNode.children)
            {
              arrayItem.children[child.first] = child.second;
            }
          }
        }

        node.array.push_back(arrayItem);
        node.isArray = true;
      }
      else
      {
        // This is a key-value pair or object
        std::string key = extractKey(trimmedLine);
        std::string value = extractValue(trimmedLine);

        YamlNode child;
        child.key = key;
        child.value = value;
        child.indentLevel = indentLevel;

        // Check if the value is an array (starts with '[' and ends with ']')
        if (value.length() >= 2 && value[0] == '[' && value[value.length() - 1] == ']')
        {
          // Parse array values
          std::string arrayContent = value.substr(1, value.length() - 2); // Remove [ and ]
          std::vector<std::string> arrayValues = split(arrayContent, ',');
          for (const std::string &arrayValue : arrayValues)
          {
            YamlNode arrayItem;
            arrayItem.value = trim(arrayValue);
            child.array.push_back(arrayItem);
          }
        }

        // Check if there are children (next line has more indentation)
        index++;
        if (index < lines.size())
        {
          int nextIndent = getIndentLevel(lines[index]);
          if (nextIndent > indentLevel)
          {
            child = parseNode(lines, index, nextIndent);
            child.key = key; // Preserve the key
          }
        }

        node.children[key] = child;

        // If this is the root level (currentIndent == 0), set the root key
        if (currentIndent == 0 && node.key.empty())
        {
          node.key = key;
        }
      }
    }
    else
    {
      // This is a child of the current node
      YamlNode child = parseNode(lines, index, indentLevel);
      if (!child.key.empty())
      {
        node.children[child.key] = child;
      }
    }
  }

  return node;
}

int YamlParser::getIndentLevel(const std::string &line)
{
  int level = 0;
  for (char c : line)
  {
    if (c == ' ' || c == '\t')
    {
      level++;
    }
    else
    {
      break;
    }
  }
  return level;
}

std::string YamlParser::trim(const std::string &str)
{
  size_t start = str.find_first_not_of(" \t\r\n");
  if (start == std::string::npos)
  {
    return "";
  }
  size_t end = str.find_last_not_of(" \t\r\n");
  return str.substr(start, end - start + 1);
}

std::vector<std::string> YamlParser::split(const std::string &str, char delimiter)
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

bool YamlParser::isArrayItem(const std::string &line)
{
  return line.length() >= 2 && line[0] == '-' && line[1] == ' ';
}

std::string YamlParser::extractKey(const std::string &line)
{
  size_t colonPos = line.find(':');
  if (colonPos != std::string::npos)
  {
    return trim(line.substr(0, colonPos));
  }
  return "";
}

std::string YamlParser::extractValue(const std::string &line)
{
  size_t colonPos = line.find(':');
  if (colonPos != std::string::npos && colonPos + 1 < line.length())
  {
    std::string value = trim(line.substr(colonPos + 1));
    // Remove surrounding quotes if present
    if (value.length() >= 2 && value[0] == '"' && value[value.length() - 1] == '"')
    {
      value = value.substr(1, value.length() - 2);
    }
    return value;
  }
  return "";
}
