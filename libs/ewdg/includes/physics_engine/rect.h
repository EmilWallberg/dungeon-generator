#ifndef RECT_H_
#define RECT_H_
#include "math/vector2.h"
#include "rigit_body_2d.h"

class Rect : public RigidBody2D {
public:
  Vector2 tl_corner, br_corner;
  Rect(Vector2 top_left_corner, Vector2 bottom_right_corner)
      : tl_corner(top_left_corner), br_corner(bottom_right_corner) {}
  bool checkCollision(const Rect &other) {
    // Handle collision between two Rect objects
    return !(
        br_corner.x < other.tl_corner.x || tl_corner.x > other.br_corner.x ||
        br_corner.y > other.tl_corner.y || tl_corner.y < other.br_corner.y);

    return false; // Default case: No collision detected
  }
};
#endif // RECT_H_
