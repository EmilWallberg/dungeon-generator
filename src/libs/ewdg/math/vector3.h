#ifndef VECTOR3_H_
#define VECTOR3_H_
#include <math.h>

namespace ewgd {
class Vector3 {
public:
  float x, y, z;

  // Constructors
  Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
  Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

  // Vector addition
  Vector3 operator+(const Vector3 &other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }

  // Vector subtraction
  Vector3 operator-(const Vector3 &other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }

  // Scalar multiplication
  Vector3 operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
  }

  // Dot product
  float dot(const Vector3 &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  // Cross product
  Vector3 cross(const Vector3 &other) const {
    return Vector3(y * other.z - z * other.y, z * other.x - x * other.z,
                   x * other.y - y * other.x);
  }

  // Magnitude (length) of the vector
  float magnitude() const { return sqrt(x * x + y * y + z * z); }

  // Normalize the vector
  Vector3 normalize() const {
    float mag = magnitude();
    if (mag != 0.0f) {
      return *this * (1.0f / mag);
    }
    return *this;
  }
};
} // namespace ewgd
#endif // VECTOR3_H_
