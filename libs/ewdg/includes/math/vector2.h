#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <compare>
#include <cstdint>
#include <math.h>

struct Vector2 {
  float x, y;

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
      // Handle division by zero
      // For simplicity, here, I set the vector components to zero
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
  friend Vector2 operator/(const Vector2 &v, float scalar) {
    if (scalar != 0) {
      return {v.x / scalar, v.y / scalar};
    } else {
      // Handle division by zero
      // For simplicity, here, I return a zero vector
      return {0, 0};
    }
  }

  // Magnitude of the vector
  float magnitude() const { return std::sqrt(x * x + y * y); }

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
    // add 2nd iteration for heigher precision
    // y  = y * (threehalfs - (x2 * y * y)); // 2nd iteration, can be optionally

    return y;
  }

  // Normalization of the vector using Fast Inverse Square Root
  Vector2 normalize() const {
    float magSquared = x * x + y * y;
    if (magSquared != 0) {
      float invMag = Q_rsqrt(magSquared);
      return {x * invMag, y * invMag};
    } else {
      // Handling zero magnitude
      // For simplicity, returning a zero vector
      return {0, 0};
    }
  }

  // Spaceship operator for comparison
  auto operator<=>(const Vector2 &other) const = default;

  friend bool operator==(const Vector2 &v1, const Vector2 &v2) {
    return v1 <=> v2 == 0;
  }

  friend bool operator!=(const Vector2 &v1, const Vector2 &v2) {
    return !(v1 == v2);
  }

  friend bool operator<(const Vector2 &v1, const Vector2 &v2) {
    return v1 <=> v2 < 0;
  }

  friend bool operator<=(const Vector2 &v1, const Vector2 &v2) {
    return !(v1 > v2);
  }

  friend bool operator>(const Vector2 &v1, const Vector2 &v2) {
    return v1 <=> v2 > 0;
  }

  friend bool operator>=(const Vector2 &v1, const Vector2 &v2) {
    return !(v1 < v2);
  }
};

#endif // VECTOR2_H_
