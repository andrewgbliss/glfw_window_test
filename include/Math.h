#pragma once

#include <vector>

// 2D Vector
struct Vector2
{
  float x, y;

  Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

  // Operators
  Vector2 operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }
  Vector2 operator-(const Vector2 &other) const { return Vector2(x - other.x, y - other.y); }
  Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
  Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

  // Assignment operators
  Vector2 &operator+=(const Vector2 &other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }
  Vector2 &operator-=(const Vector2 &other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  Vector2 &operator*=(float scalar)
  {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  Vector2 &operator/=(float scalar)
  {
    x /= scalar;
    y /= scalar;
    return *this;
  }
};

// 3D Vector
struct Vector3
{
  float x, y, z;

  Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

  // Operators
  Vector3 operator+(const Vector3 &other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
  Vector3 operator-(const Vector3 &other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
  Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
  Vector3 operator/(float scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }

  // Assignment operators
  Vector3 &operator+=(const Vector3 &other)
  {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
  Vector3 &operator-=(const Vector3 &other)
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }
  Vector3 &operator*=(float scalar)
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }
  Vector3 &operator/=(float scalar)
  {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }
};

// Position (alias for Vector3)
using Position = Vector3;

// Scale (alias for Vector3)
using Scale = Vector3;

// Color (alias for Vector3)
using Color = Vector3;

// 2D Position and Scale aliases
using Position2D = Vector2;
using Scale2D = Vector2;

// Transform2D struct that combines 2D position, scale, and rotation
struct Transform2D
{
  Position2D position;
  Scale2D scale;
  float rotation; // Rotation in degrees

  Transform2D(const Position2D &pos = Position2D(),
              const Scale2D &scl = Scale2D(1.0f, 1.0f),
              float rot = 0.0f)
      : position(pos), scale(scl), rotation(rot) {}

  // Setters
  void setPosition(const Position2D &pos) { position = pos; }
  void setScale(const Scale2D &scl) { scale = scl; }
  void setRotation(float rot) { rotation = rot; }

  // Getters
  const Position2D &getPosition() const { return position; }
  const Scale2D &getScale() const { return scale; }
  float getRotation() const { return rotation; }
};

// Transform3D struct that combines 3D position, scale, and rotation
struct Transform3D
{
  Position position;
  Scale scale;
  Vector3 rotation; // Euler angles in degrees

  Transform3D(const Position &pos = Position(),
              const Scale &scl = Scale(1.0f, 1.0f, 1.0f),
              const Vector3 &rot = Vector3())
      : position(pos), scale(scl), rotation(rot) {}

  // Setters
  void setPosition(const Position &pos) { position = pos; }
  void setScale(const Scale &scl) { scale = scl; }
  void setRotation(const Vector3 &rot) { rotation = rot; }

  // Getters
  const Position &getPosition() const { return position; }
  const Scale &getScale() const { return scale; }
  const Vector3 &getRotation() const { return rotation; }
};

// Static Colors class with predefined colors
class Colors
{
public:
  // Basic colors
  static const Color red;
  static const Color green;
  static const Color blue;
  static const Color yellow;
  static const Color cyan;
  static const Color magenta;
  static const Color white;
  static const Color black;
  static const Color gray;
  static const Color orange;
  static const Color purple;
  static const Color pink;
  static const Color brown;
  static const Color lime;
  static const Color navy;
  static const Color teal;
  static const Color gold;
  static const Color silver;
  static const Color indigo;
  static const Color violet;
};

// Define the static color constants
inline const Color Colors::red = Color(1.0f, 0.0f, 0.0f);
inline const Color Colors::green = Color(0.0f, 1.0f, 0.0f);
inline const Color Colors::blue = Color(0.0f, 0.0f, 1.0f);
inline const Color Colors::yellow = Color(1.0f, 1.0f, 0.0f);
inline const Color Colors::cyan = Color(0.0f, 1.0f, 1.0f);
inline const Color Colors::magenta = Color(1.0f, 0.0f, 1.0f);
inline const Color Colors::white = Color(1.0f, 1.0f, 1.0f);
inline const Color Colors::black = Color(0.0f, 0.0f, 0.0f);
inline const Color Colors::gray = Color(0.5f, 0.5f, 0.5f);
inline const Color Colors::orange = Color(1.0f, 0.5f, 0.0f);
inline const Color Colors::purple = Color(0.5f, 0.0f, 0.5f);
inline const Color Colors::pink = Color(1.0f, 0.75f, 0.8f);
inline const Color Colors::brown = Color(0.6f, 0.4f, 0.2f);
inline const Color Colors::lime = Color(0.5f, 1.0f, 0.0f);
inline const Color Colors::navy = Color(0.0f, 0.0f, 0.5f);
inline const Color Colors::teal = Color(0.0f, 0.5f, 0.5f);
inline const Color Colors::gold = Color(1.0f, 0.84f, 0.0f);
inline const Color Colors::silver = Color(0.75f, 0.75f, 0.75f);
inline const Color Colors::indigo = Color(0.29f, 0.0f, 0.51f);
inline const Color Colors::violet = Color(0.5f, 0.0f, 1.0f);