#ifndef VECTOR3_H_
#define VECTOR3_H_
#include <math.h>

namespace ewdg {
class Vector3 {
public:
  double x, y, z;

  // Constructors
  Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
  Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

  // Vector addition
  Vector3 operator+(const Vector3 &other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }

  // Vector subtraction
  Vector3 operator-(const Vector3 &other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }

  // Scalar multiplication
  Vector3 operator*(double scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
  }

  // Dot product
  double dot(const Vector3 &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  // Cross product
  Vector3 cross(const Vector3 &other) const {
    return Vector3(y * other.z - z * other.y, z * other.x - x * other.z,
                   x * other.y - y * other.x);
  }

  // Length (magnitude) of the vector
  double length() const { return sqrt(x * x + y * y + z * z); }

  // Normalize the vector
  Vector3 normalize() const {
    double mag = length();
    if (mag != 0.0f) {
      return *this * (1.0f / mag);
    }
    return *this;
  }
};
} // namespace ewdg
#endif // VECTOR3_H_
