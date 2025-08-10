#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream> // Added for debug output

class Input
{
private:
  static Input *instance;

  // Action mapping: action_name -> key_code
  std::unordered_map<std::string, int> actionMap;

  // Current frame key states
  std::unordered_map<int, bool> currentKeyStates;

  // Previous frame key states (for "just pressed" detection)
  std::unordered_map<int, bool> previousKeyStates;

  // Private constructor for singleton
  Input() = default;

public:
  // Singleton access
  static Input &getInstance();

  // Prevent copying
  Input(const Input &) = delete;
  Input &operator=(const Input &) = delete;

  // Setup action mappings
  void setupAction(const std::string &actionName, int keyCode);

  // Update input states (call this once per frame)
  void update();

  // Set key state (to be called by platform-specific input handler)
  void setKeyState(int keyCode, bool pressed);

  // Check if an action is currently pressed
  bool isActionPressed(const std::string &actionName) const;

  // Check if an action was just pressed this frame
  bool isActionJustPressed(const std::string &actionName) const;

  // Check if an action was just released this frame
  bool isActionJustReleased(const std::string &actionName) const;

  // Direct key checking (if needed)
  bool isKeyPressed(int keyCode) const;
  bool isKeyJustPressed(int keyCode) const;
  bool isKeyJustReleased(int keyCode) const;

  // Get the key code for an action
  int getActionKey(const std::string &actionName) const;

  // Clear all action mappings
  void clearActions();
};

// Implementation
inline Input &Input::getInstance()
{
  if (instance == nullptr)
  {
    instance = new Input();
  }
  return *instance;
}

inline void Input::setupAction(const std::string &actionName, int keyCode)
{
  actionMap[actionName] = keyCode;
  std::cout << "Action mapping created: '" << actionName << "' -> key " << keyCode << std::endl;
}

inline void Input::update()
{
  // Store previous frame states
  previousKeyStates = currentKeyStates;
}

inline void Input::setKeyState(int keyCode, bool pressed)
{
  currentKeyStates[keyCode] = pressed;
  if (pressed)
  {
    std::cout << "Input class: Key " << keyCode << " set to PRESSED" << std::endl;
  }
}

inline bool Input::isActionPressed(const std::string &actionName) const
{
  auto it = actionMap.find(actionName);
  if (it != actionMap.end())
  {
    auto keyIt = currentKeyStates.find(it->second);
    bool isPressed = keyIt != currentKeyStates.end() && keyIt->second;
    if (isPressed)
    {
      std::cout << "Action '" << actionName << "' (key " << it->second << ") is PRESSED" << std::endl;
    }
    return isPressed;
  }
  std::cout << "Action '" << actionName << "' not found in action map!" << std::endl;
  return false;
}

inline bool Input::isActionJustPressed(const std::string &actionName) const
{
  auto it = actionMap.find(actionName);
  if (it != actionMap.end())
  {
    int keyCode = it->second;
    auto currentIt = currentKeyStates.find(keyCode);
    auto previousIt = previousKeyStates.find(keyCode);

    bool currentlyPressed = currentIt != currentKeyStates.end() && currentIt->second;
    bool previouslyPressed = previousIt != previousKeyStates.end() && previousIt->second;

    return currentlyPressed && !previouslyPressed;
  }
  return false;
}

inline bool Input::isActionJustReleased(const std::string &actionName) const
{
  auto it = actionMap.find(actionName);
  if (it != actionMap.end())
  {
    int keyCode = it->second;
    auto currentIt = currentKeyStates.find(keyCode);
    auto previousIt = previousKeyStates.find(keyCode);

    bool currentlyPressed = currentIt != currentKeyStates.end() && currentIt->second;
    bool previouslyPressed = previousIt != previousKeyStates.end() && previousIt->second;

    return !currentlyPressed && previouslyPressed;
  }
  return false;
}

inline bool Input::isKeyPressed(int keyCode) const
{
  auto it = currentKeyStates.find(keyCode);
  return it != currentKeyStates.end() && it->second;
}

inline bool Input::isKeyJustPressed(int keyCode) const
{
  auto currentIt = currentKeyStates.find(keyCode);
  auto previousIt = previousKeyStates.find(keyCode);

  bool currentlyPressed = currentIt != currentKeyStates.end() && currentIt->second;
  bool previouslyPressed = previousIt != previousKeyStates.end() && previousIt->second;

  return currentlyPressed && !previouslyPressed;
}

inline bool Input::isKeyJustReleased(int keyCode) const
{
  auto currentIt = currentKeyStates.find(keyCode);
  auto previousIt = previousKeyStates.find(keyCode);

  bool currentlyPressed = currentIt != currentKeyStates.end() && currentIt->second;
  bool previouslyPressed = previousIt != previousKeyStates.end() && previousIt->second;

  return !currentlyPressed && previouslyPressed;
}

inline int Input::getActionKey(const std::string &actionName) const
{
  auto it = actionMap.find(actionName);
  return it != actionMap.end() ? it->second : -1;
}

inline void Input::clearActions()
{
  actionMap.clear();
  currentKeyStates.clear();
  previousKeyStates.clear();
}

// Initialize static member
inline Input *Input::instance = nullptr;
