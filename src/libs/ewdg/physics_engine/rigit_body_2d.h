#ifndef RIGID_BODY_2D_H_
#define RIGID_BODY_2D_H_

#include "math/vector2.h"
#include <iostream>

namespace ewdg {
class RigidBody2D {
public:
  Vector2 position;
  Vector2 velocity;
  float mass;

  RigidBody2D(Vector2 position = Vector2(), Vector2 velocity = Vector2(),
              float mass = 1.0f)
      : position(position), velocity(velocity), mass(mass) {} // Constructor
  // Methods for setting/getting properties or performing operations
  void set_position(const Vector2 &new_position) { position = new_position; }
  Vector2 get_position() const { return position; }
  void apply_force(const Vector2 &force, float delta) {
    // Applying force: F = ma (assuming constant mass)
    // Calculate acceleration using F = ma -> a = F / m
    Vector2 acceleration = force / mass;

    // Update velocity using the calculated acceleration
    velocity += acceleration * delta;
    if (velocity.length() < 0.0001)
      velocity = Vector2(0, 0);
  }
  void simulate(float delta) {
    position += velocity * delta;
    // std::printf("V: (%f, %f)\n", velocity.x, velocity.y);
  }
};
} // namespace ewdg
#endif // RIGID_BODY_2D_H_
