
#ifndef RIGID_BODY_2D_H_
#define RIGID_BODY_2D_H_

#include "math/vector2.h" // Assuming Vector2 is a separate class

class RigidBody2D {
public:
  RigidBody2D()
      : m_position(0, 0), m_velocity(0, 0), m_mass(1.0f) {} // Constructor

  // Methods for setting/getting properties or performing operations
  void set_position(const Vector2 &new_position) { m_position = new_position; }
  Vector2 get_position() const { return m_position; }
  void apply_force(const Vector2 &force, float delta) {
    // Applying force: F = ma (assuming constant mass)
    // Calculate acceleration using F = ma -> a = F / m
    Vector2 acceleration = force / m_mass;

    // Update velocity using the calculated acceleration
    m_velocity += acceleration * delta;
  }
  // Other physics-related methods

private:
  Vector2 m_position;
  Vector2 m_velocity;
  float m_mass;
  // Other properties or data relevant to a 2D rigid body
};

#endif // RIGID_BODY_2D_H_
