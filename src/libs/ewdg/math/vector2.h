#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <cmath>
#include <cstdint>
#include <cstdio>

namespace ewdg {
struct Vector2 {
  double x, y;
  Vector2(double x, double y) : x(x), y(y) {}
  Vector2(const Vector2 &vec) : x(vec.x), y(vec.y) {}
  Vector2() {
    x = 0.0f;
    y = 0.0f;
  }
  Vector2 &operator=(const Vector2 &other) {
    if (this != &other) {
      x = other.x;
      y = other.y;
    }
    return *this;
  }
  // Compound assignment operators
  friend void operator+=(Vector2 &v1, const Vector2 &v2) {
    v1.x += v2.x;
    v1.y += v2.y;
  }

  friend void operator-=(Vector2 &v1, const Vector2 &v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
  }

  friend void operator*=(Vector2 &v, float scalar) {
    v.x *= scalar;
    v.y *= scalar;
  }

  friend void operator/=(Vector2 &v, float scalar) {
    if (scalar != 0) {
      v.x /= scalar;
      v.y /= scalar;
    } else {
      v.x = 0;
      v.y = 0;
    }
  }

  // Unary negation operator
  Vector2 operator-() const { return {-x, -y}; }

  // Binary addition operator
  friend Vector2 operator+(const Vector2 &v1, const Vector2 &v2) {
    return {v1.x + v2.x, v1.y + v2.y};
  }

  // Binary subtraction operator
  friend Vector2 operator-(const Vector2 &v1, const Vector2 &v2) {
    return {v1.x - v2.x, v1.y - v2.y};
  }

  // Scalar multiplication operator
  friend Vector2 operator*(const Vector2 &v, float scalar) {
    return {v.x * scalar, v.y * scalar};
  }

  friend Vector2 operator*(float scalar, const Vector2 &v) {
    return v * scalar;
  }

  // Scalar division operator
  friend Vector2 operator/(const Vector2 &v, double scalar) {
    if (scalar != 0) {
      return {v.x / scalar, v.y / scalar};
    } else {
      return {0, 0};
    }
  }
  friend void swap(Vector2 &lhs, Vector2 &rhs) {
    std::swap(lhs.x, rhs.x);
    std::swap(lhs.y, rhs.y);
  }
  // Magnitude of the vector
  float length() const { return std::sqrt(x * x + y * y); }

  Vector2 perpendicular() const {
    // Rotate the vector by 90 degrees counterclockwise
    return {-y, x};
  }

  // Quake III Arena's Fast Inverse Square Root
  static float Q_rsqrt(float number) {
    uint32_t i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(uint32_t *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (threehalfs - (x2 * y * y)); // 1st iteration

    return y;
  }

  // Normalization of the vector using Fast Inverse Square Root
  Vector2 normalize() const {
    float magSquared = x * x + y * y;
    if (magSquared != 0) {
      float invMag = Q_rsqrt(magSquared);
      return {x * invMag, y * invMag};
    } else {
      return {0, 0};
    }
  }
  float cross(const Vector2 &v1) { return x * v1.y - y * v1.x; }
  // Dot product function
  float dot(const Vector2 &v1) { return x * v1.x + y * v1.y; }

  static float distance_point_to_line(const Vector2 &p0, const Vector2 &p1,
                                      const Vector2 &p2) {
    float x0 = p0.x, y0 = p0.y;
    float x1 = p1.x, y1 = p1.y;
    float x2 = p2.x, y2 = p2.y;

    // Calculate the distance from p2 to the line formed by p0 and p1
    float distance = std::abs((y2 - y0) * (x1 - x0) - (x2 - x0) * (y1 - y0)) /
                     sqrt((y1 - y0) * (y1 - y0) + (x1 - x0) * (x1 - x0));

    return distance;
  }

  static bool is_point_on_line(const Vector2 &p0, const Vector2 &p1,
                               const Vector2 &p2, float accuracy = 0.001) {
    // Check if the distance from p2 to the line is within the specified
    // accuracy
    return distance_point_to_line(p0, p1, p2) < accuracy;
  }

  // Comparison operators
  friend bool operator==(const Vector2 &v1, const Vector2 &v2) {
    // Define a tolerance for floating-point comparison
    const double epsilon = 1e-8; // You can adjust this value as needed

    // Check if the difference between components is within the tolerance
    return std::abs(v1.x - v2.x) < epsilon && std::abs(v1.y - v2.y) < epsilon;
  }

  friend bool operator!=(const Vector2 &v1, const Vector2 &v2) {
    return !(v1 == v2);
  }

  friend bool operator<(const Vector2 &v1, const Vector2 &v2) {
    if (v1.x != v2.x) {
      return v1.x < v2.x;
    }
    return v1.y < v2.y;
  }

  friend bool operator<=(const Vector2 &v1, const Vector2 &v2) {
    return !(v2 < v1);
  }

  friend bool operator>(const Vector2 &v1, const Vector2 &v2) {
    return v2 < v1;
  }

  friend bool operator>=(const Vector2 &v1, const Vector2 &v2) {
    return !(v1 < v2);
  }
  const char *toString() const {
    static char buffer[50];
    snprintf(buffer, sizeof(buffer), "(%.2f, %.2f)", x, y);
    return buffer;
  }
};
} // namespace ewdg
#endif // VECTOR2_H_
