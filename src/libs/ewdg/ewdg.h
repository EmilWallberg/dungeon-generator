#ifndef EWDG_H_
#define EWDG_H_
#include "math/delaunay_triangulation.h"
#include "math/vector2.h"
#include "path.h"
#include "physics_engine/rect.h"
#include "room.h"

#include <random>
#include <vector>

namespace ewdg {
class Dungeon {
public:
  std::vector<Room> rooms{};
  std::vector<Room> main_rooms{};
  std::vector<Path> paths{};
  DelaunayTriangulation<Room> delaunay;
  std::set<Edge<Room>> dungeon_layout{};
  Vector2 dungeon_bounds = Vector2(50.0f, 50.0f);

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
    bool simulation_done = false;
    while (!simulation_done) {
      simulation_done = time_step_rooms(repulsion_force, friction_force, delta);
    }
#ifdef DEBUG_ENABLED
    std::printf("Physics simulation done\n");
#endif
  }

  bool time_step_rooms(float repulsion_force, float friction_force,
                       float delta) {
    bool moving = false;
    bool colliding = false;
    for (int i = 0; i < rooms.size(); i++) {
      Rect *rbi = &rooms[i];
      for (int j = i + 1; j < rooms.size(); j++) {
        Rect *rbj = &rooms[j];
        if (rbi->checkCollision(*rbj)) {
          colliding = true;
          Vector2 force_dir = (rbj->position - rbi->position).normalize();
          Vector2 force = force_dir * repulsion_force;
          rbi->apply_force(-force, delta);
          rbj->apply_force(force, delta);
        }
      }
      if (rbi->velocity != Vector2(0, 0))
        rbi->apply_force(-rbi->velocity * friction_force, delta);
    }
    for (Room &r : rooms) {
      r.simulate(delta);
      if (r.velocity != Vector2(0, 0)) {
        moving = true;
      }
    }
    return !(colliding || moving);
#ifdef DEBUG_ENABLED
    std::printf("Physics simulation done\n");
#endif
  }

  // TODO: Make mesh class for easyer mesh operations
  std::pair<std::vector<Vector3>, std::vector<int32_t>>
  generate_mesh(bool main_rooms_only) {
    std::vector<Vector3> vertices;
    std::vector<int32_t> indices;
    if (!main_rooms_only) {
      for (const Room &r : rooms) {
        r.generate_3d_mesh(vertices, indices);
      }
    }

    for (const Room &r : main_rooms) {
      r.generate_3d_mesh(vertices, indices);
    }

    for (const Path &p : paths) {
      p.generate_3d_mesh(vertices, indices);
    }
    return std::make_pair(vertices, indices);
  }

  void generate_paths() {
    for (const Edge<Room> &e : dungeon_layout) {
      paths.push_back(Path(*e.from, *e.to));
    }
  }

  void make_graf_layout(int main_room_count, int extra_paths_count) {
    populate_main_room_vector(main_room_count);
    delaunay.brutforce_graf(main_rooms);
    dungeon_layout = delaunay.generate_minimum_spanning_tree();

    std::set<Edge<Room>> difference;
    std::set_difference(delaunay.edges.begin(), delaunay.edges.end(),
                        dungeon_layout.begin(), dungeon_layout.end(),
                        std::inserter(difference, difference.begin()));

    // Seed random number generation
    std::random_device rd;
    std::mt19937 gen(rd());

    // Add random elements from 'difference' to 'dungeon_layout'
    int count = 0;
    while (count++ < extra_paths_count && !difference.empty()) {
      std::uniform_int_distribution<int> dis(0, difference.size() - 1);
      auto it = std::next(std::begin(difference), dis(gen));
      dungeon_layout.insert(*it);
      difference.erase(it);
    }
  }

private:
  void populate_main_room_vector(size_t main_room_count) {
    // First, sort the rooms by area in ascending order
    std::sort(rooms.begin(), rooms.end(), [](const Room &a, const Room &b) {
      return a.get_area() < b.get_area();
    });

    main_room_count =
        rooms.size() > main_room_count ? main_room_count : rooms.size();
    std::copy(rooms.end() - main_room_count, rooms.end(),
              std::back_inserter(main_rooms));

    rooms.erase(rooms.end() - main_room_count, rooms.end());
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
      rb.position = r.position;
    }
  };
};
} // namespace ewdg
#endif // EWDG_H_
