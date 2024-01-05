#ifndef RECT_H_
#define RECT_H_
#include "math/vector2.h"
#include "rigit_body_2d.h"
namespace ewdg {
class Rect : public RigidBody2D {
public:
  float width;
  float height;
  // Functions to get room corners based on center, width, and height
  Vector2 get_topleft_corner() const {
    return position + Vector2(-width / 2, -height / 2);
  }

  Vector2 get_topright_corner() const {
    return position + Vector2(width / 2, -height / 2);
  }

  Vector2 get_bottomright_corner() const {
    return position + Vector2(width / 2, height / 2);
  }

  Vector2 get_bottomleft_corner() const {
    return position + Vector2(-width / 2, height / 2);
  }

  double get_area() const { return width * height; };

  Rect(float width, float height, Vector2 position = Vector2(),
       Vector2 velocity = Vector2(), float mass = 1.0f)
      : RigidBody2D(position, velocity, mass), width(width), height(height) {}

  bool checkCollision(const Rect &other) {
    Vector2 br_corner = get_bottomright_corner();
    Vector2 tl_corner = get_topleft_corner();
    Vector2 other_br_corner = other.get_bottomright_corner();
    Vector2 other_tl_corner = other.get_topleft_corner();

    return !(
        br_corner.x < other_tl_corner.x || tl_corner.x > other_br_corner.x ||
        br_corner.y < other_tl_corner.y || tl_corner.y > other_br_corner.y);
  }
};
} // namespace ewdg
#endif // RECT_H_
