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

private:
  Vector2 dungeon_bounds = Vector2(100.0f, 100.0f);
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

  void generate_rooms(int room_count, float min_width, float max_width) {
    for (int i = 0; i < room_count; i++) {
      Vector2 center_position = generate_random_position(
          dungeon_bounds); // TODO: Makit so a function for random numbers can
                           // be pasted to this function
      float room_width = random_float(min_width, max_width);
      float room_height = random_float(min_width, max_width);

      rooms.push_back(Room(center_position, room_width, room_height));
    }
  }

  void simulate_rooms(float repulsion_force, float delta) {
    struct rb_room {
      Room &room;
      Rect rb;
      rb_room(Room &r)
          : room(r),
            rb(Rect(r.get_topleft_corner(), r.get_bottomright_corner())) {}
    };
    std::vector<rb_room> rbs;
    for (int i = 0; i < rooms.size(); i++) {
      rbs.push_back(rb_room(rooms[i]));
    }

    bool collision = false;
    do {
      collision = false;
      for (int i = 0; i < rbs.size(); i++) {
        Rect rbi = rbs[i].rb;
        for (int j = i + 1; j < rbs.size(); j++) {
          Rect rbj = rbs[i].rb;
          if (rbi.checkCollision(rbj)) {
            Vector2 force_dir =
                (rbs[j].room.center - rbs[i].room.center).normalize();
            Vector2 force = force_dir * repulsion_force * delta;
            rbi.apply_force()
          }
        }
      }
    } while (collision);
  }
};
} // namespace ewgd