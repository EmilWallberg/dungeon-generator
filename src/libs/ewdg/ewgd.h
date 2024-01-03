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
  Vector2 dungeon_bounds = Vector2(250.0f, 250.0f);

  void generate_rooms(int room_count, float min_width, float max_width) {
    for (int i = 0; i < room_count; i++) {
      Vector2 center_position = generate_random_position(
          dungeon_bounds); // TODO: Make it so a function for random numbers can
                           // be pasted to this function
      float room_width = random_float(min_width, max_width);
      float room_height = random_float(min_width, max_width);

      rooms.push_back(Room(center_position, room_width, room_height));
    }
#ifdef DEBUG_ENABLED
    std::printf("Room count: %zi", size(rooms));
#endif
  }

  void simulate_rooms(float repulsion_force, float friction_force,
                      float delta) {
    std::vector<rb_room> rbs;
    for (int i = 0; i < rooms.size(); i++) {
      rbs.push_back(rb_room(rooms[i]));
    }
    bool moving = false;
    bool colliding = false;
    do {
      colliding = false;
      for (int i = 0; i < rbs.size(); i++) {
        Rect *rbi = &rbs[i].rb;
        for (int j = i + 1; j < rbs.size(); j++) {
          Rect *rbj = &rbs[j].rb;
          if (rbi->checkCollision(*rbj)) {
            colliding = true;
            Vector2 force_dir = (rbj->position - rbi->position).normalize();
            Vector2 force = force_dir * repulsion_force;
            rbi->apply_force(force, delta);
            rbj->apply_force(-force, delta);
          }
        }
        if (rbi->velocity != Vector2(0, 0))
          rbi->apply_force(-rbi->velocity.normalize() * friction_force, delta);
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
    } while (colliding || moving);
#ifdef DEBUG_ENABLED
    std::printf("Physics simulation done\n");
#endif
  }

  bool time_step_rooms(float repulsion_force, float friction_force,
                       float delta) {
    std::vector<rb_room> rbs;
    for (int i = 0; i < rooms.size(); i++) {
      rbs.push_back(rb_room(rooms[i]));
    }
    bool moving = false;
    bool colliding = false;
    colliding = false;
    for (int i = 0; i < rbs.size(); i++) {
      Rect *rbi = &rbs[i].rb;
      for (int j = i + 1; j < rbs.size(); j++) {
        Rect *rbj = &rbs[j].rb;
        if (rbi->checkCollision(*rbj)) {
          colliding = true;
          Vector2 force_dir = (rbj->position - rbi->position).normalize();
          Vector2 force = force_dir * repulsion_force;
          rbi->apply_force(-force, delta);
          rbj->apply_force(force, delta);
        }
      }
      if (rbi->velocity != Vector2(0, 0))
        rbi->apply_force(-rbi->velocity.normalize() * friction_force, delta);
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
    return !(colliding || moving);
#ifdef DEBUG_ENABLED
    std::printf("Physics simulation done\n");
#endif
  }
  std::pair<std::vector<Vector3>, std::vector<int32_t>> generate_mesh() {
    std::vector<Vector3> vertices;
    std::vector<int32_t> indices;
    for (const Room &r : rooms) {
      r.generate_3d_mesh(vertices, indices);
    }
    return std::make_pair(vertices, indices);
  }

private:
  std::vector<Room *> main_room_pointer_vector(float area_threshhold) {
    std::vector<Room *> main_rooms;
    for (Room &r : rooms) {
      if (r.get_area() > area_threshhold) {
        main_rooms.push_back(&r);
      }
    }
    return main_rooms;
  }

  float random_float(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
  }

  Vector2 generate_random_position(const Vector2 &bounds) {
    float x = random_float(-bounds.x / 2, bounds.x / 2);
    float y = random_float(-bounds.y / 2, bounds.y / 2);
    return Vector2(x, y);
  }

  struct rb_room {
    Room &room;
    Rect rb;
    rb_room(Room &r) : room(r), rb(Rect(r.width, r.height)) {
      rb.position = r.center;
    }
  };
};
} // namespace ewgd
#endif // EWDG_H_
