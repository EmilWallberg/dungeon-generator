#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include "libs/ewdg/ewdg.h"
#include <godot_cpp/classes/mesh_instance3d.hpp>

namespace godot {

class GDExample : public MeshInstance3D {
  GDCLASS(GDExample, MeshInstance3D);

private:
  double timer;
  int room_to_be_generated = 50, main_room_count = 25;
  double simulation_timestep = 0.1;
  double repultion_force = 1;
  double friction_force = 0.5;
  Vector2 min_max_room_width = {5, 20};
  ewdg::Dungeon d;
  bool simulation_done = false;
  bool graf_done = false;
  bool dungeon_done = false;

protected:
  static void _bind_methods() {
    // Rooms to be generated
    ClassDB::bind_method(D_METHOD("get_rooms_to_be_generated"),
                         &GDExample::get_rooms_to_be_generated);

    ClassDB::bind_method(
        D_METHOD("set_rooms_to_be_generated", "p_room_to_be_generated"),
        &GDExample::set_rooms_to_be_generated);

    ClassDB::add_property(
        "GDExample", PropertyInfo(Variant::INT, "rooms_to_be_generated"),
        "set_rooms_to_be_generated", "get_rooms_to_be_generated");

    // Main rooms to be selected
    ClassDB::bind_method(D_METHOD("get_main_room_count"),
                         &GDExample::get_main_room_count);

    ClassDB::bind_method(D_METHOD("set_main_room_count", "p_main_room_count"),
                         &GDExample::set_main_room_count);

    ClassDB::add_property("GDExample",
                          PropertyInfo(Variant::INT, "main_room_count"),
                          "set_main_room_count", "get_main_room_count");

    // Simulation timestep
    ClassDB::bind_method(D_METHOD("get_simulation_timestep"),
                         &GDExample::get_simulation_timestep);
    ClassDB::bind_method(
        D_METHOD("set_simulation_timestep", "p_simulation_timestep"),
        &GDExample::set_simulation_timestep);
    ClassDB::add_property("GDExample",
                          PropertyInfo(Variant::FLOAT, "simulation_timestep",
                                       PROPERTY_HINT_RANGE, "0.001,1,0.001"),
                          "set_simulation_timestep", "get_simulation_timestep");
    // min max room width
    ClassDB::bind_method(D_METHOD("get_min_max_room_width"),
                         &GDExample::get_min_max_room_width);

    ClassDB::bind_method(
        D_METHOD("set_min_max_room_width", "p_min_max_room_width"),
        &GDExample::set_min_max_room_width);

    ClassDB::add_property("GDExample",
                          PropertyInfo(Variant::VECTOR2, "min_max_room_width"),
                          "set_min_max_room_width", "get_min_max_room_width");
    // repultion_force timestep
    ClassDB::bind_method(D_METHOD("get_repultion_force"),
                         &GDExample::get_repultion_force);
    ClassDB::bind_method(D_METHOD("set_repultion_force", "p_repultion_force"),
                         &GDExample::set_repultion_force);
    ClassDB::add_property("GDExample",
                          PropertyInfo(Variant::FLOAT, "repultion_force",
                                       PROPERTY_HINT_RANGE, "0.001,10,0.001"),
                          "set_repultion_force", "get_repultion_force");
    // friction_force timestep
    ClassDB::bind_method(D_METHOD("get_friction_force"),
                         &GDExample::get_friction_force);
    ClassDB::bind_method(D_METHOD("set_friction_force", "p_friction_force"),
                         &GDExample::set_friction_force);
    ClassDB::add_property("GDExample",
                          PropertyInfo(Variant::FLOAT, "friction_force",
                                       PROPERTY_HINT_RANGE, "0.001,10,0.001"),
                          "set_friction_force", "get_friction_force");
  };

public:
  GDExample();
  ~GDExample();

  void line(ewdg::Vector3 pos1, ewdg::Vector3 pos2,
            Color c = Color(1, 0, 0, 1));
  void _ready() override;
  void _process(double delta) override;

  void set_rooms_to_be_generated(const int p_room_to_be_generated) {
    room_to_be_generated = p_room_to_be_generated;
  }
  int get_rooms_to_be_generated() const { return room_to_be_generated; };

  void set_main_room_count(const int p_main_room_count) {
    main_room_count = p_main_room_count;
  }
  int get_main_room_count() const { return main_room_count; };

  void set_min_max_room_width(const Vector2 p_min_max_room_width) {
    min_max_room_width = p_min_max_room_width;
  }
  Vector2 get_min_max_room_width() const { return min_max_room_width; };

  void set_simulation_timestep(const double p_simulation_timestep) {
    simulation_timestep = p_simulation_timestep;
  }

  double get_simulation_timestep() const { return simulation_timestep; }

  void set_repultion_force(const double p_repultion_force) {
    repultion_force = p_repultion_force;
  }

  double get_repultion_force() const { return repultion_force; }

  void set_friction_force(const double p_friction_force) {
    friction_force = p_friction_force;
  }

  double get_friction_force() const { return friction_force; }

private:
  std::vector<MeshInstance3D *> mesh_instances{};
};

} // namespace godot

#endif
