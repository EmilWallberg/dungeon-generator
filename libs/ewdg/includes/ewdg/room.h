#ifndef ROOM_H_
#define ROOM_H_

#include "math/vector2.h"
namespace ewgd {
class Room {
public:
  Vector2 center;
  float width;
  float height;

  Room(const Vector2 &c, float w, float h) : center(c), width(w), height(h) {}

  // Functions to get room corners based on center, width, and height
  Vector2 get_topleft_corner() const {
    return center + Vector2(-width / 2, -height / 2);
  }

  Vector2 get_topright_corner() const {
    return center + Vector2(width / 2, -height / 2);
  }

  Vector2 get_bottomright_corner() const {
    return center + Vector2(width / 2, height / 2);
  }

  Vector2 get_bottomleft_corner() const {
    return center + Vector2(-width / 2, height / 2);
  }
};
} // namespace ewgd
#endif // ROOM_H_
