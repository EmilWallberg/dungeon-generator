
#ifndef RIGID_BODY_2D_H_
#define RIGID_BODY_2D_H_

#include "math/vector2.h" // Assuming Vector2 is a separate class
#include <iostream>

namespace ewgd {
class RigidBody2D {
public:
  Vector2 position;
  Vector2 velocity;
  float mass;

  RigidBody2D()
      : position(Vector2()), velocity(Vector2()), mass(1.0f) {} // Constructor

  // Methods for setting/getting properties or performing operations
  void set_position(const Vector2 &new_position) { position = new_position; }
  Vector2 get_position() const { return position; }
  void apply_force(const Vector2 &force, float delta) {
    // Applying force: F = ma (assuming constant mass)
    // Calculate acceleration using F = ma -> a = F / m
    Vector2 acceleration = force / mass;

    // Update velocity using the calculated acceleration
    velocity += acceleration * delta;
    if (velocity.magnitude() < 0.0001)
      velocity = Vector2(0, 0);
  }
  void simulate(float delta) {
    position += velocity * delta;
    // std::printf("V: (%f, %f)\n", velocity.x, velocity.y);
  }
};
} // namespace ewgd
#endif // RIGID_BODY_2D_H_
