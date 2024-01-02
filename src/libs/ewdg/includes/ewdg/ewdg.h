#ifndef EWDG_H_
#define EWDG_H_
#include "math/vector2.h"
#include "physics_engine/rect.h"
#include "room.h"
#include <random>
#include <vector>

namespace ewgd {
class Dungeon {
public:
  std::vector<Room> rooms;
  Vector2 dungeon_bounds = Vector2(100.0f, 100.0f);

  void generate_rooms(int room_count, float min_width, float max_width) {
    for (int i = 0; i < room_count; i++) {
      Vector2 center_position = generate_random_position(
          dungeon_bounds); // TODO: Make it so a function for random numbers can
                           // be pasted to this function
      float room_width = random_float(min_width, max_width);
      float room_height = random_float(min_width, max_width);

      rooms.push_back(Room(center_position, room_width, room_height));
    }
  }

  void simulate_rooms(float repulsion_force, float friction_force,
                      float delta) {
    std::vector<rb_room> rbs;
    for (int i = 0; i < rooms.size(); i++) {
      rbs.push_back(rb_room(rooms[i]));
    }
    bool moving = false;
    do {
      for (int i = 0; i < rbs.size(); i++) {
        Rect rbi = rbs[i].rb;
        for (int j = i + 1; j < rbs.size(); j++) {
          Rect rbj = rbs[i].rb;
          if (rbi.checkCollision(rbj)) {
            Vector2 force_dir =
                (rbs[j].room.center - rbs[i].room.center).normalize();
            Vector2 force = force_dir * repulsion_force;
            rbi.apply_force(force, delta);
            rbj.apply_force(force, delta);
          }
          rbi.apply_force(-rbi.velocity.normalize() * friction_force, delta);
          rbj.apply_force(-rbj.velocity.normalize() * friction_force, delta);
        }
      }
      moving = false;
      for (int i = 0; i < rbs.size(); i++) {
        rb_room rr = rbs[i];
        rr.rb.simulate(delta);
        rr.room.center = rr.rb.position;
        if (rr.rb.velocity != Vector2(0, 0)) {
          moving = true;
        }
      }
    } while (moving);
  }

private:
  float random_float(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
  }

  Vector2 generate_random_position(const Vector2 &bounds) {
    float x = random_float(0, bounds.x);
    float y = random_float(0, bounds.y);
    return Vector2(x, y);
  }

  struct rb_room {
    Room &room;
    Rect rb;
    rb_room(Room &r) : room(r), rb(Rect(r.width, r.height)) {}
  };
};
} // namespace ewgd
#endif // EWDG_H_
