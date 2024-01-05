#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include "libs/ewdg/ewdg.h"
#include <godot_cpp/classes/mesh_instance3d.hpp>

namespace godot {

class GDExample : public MeshInstance3D {
  GDCLASS(GDExample, MeshInstance3D);

private:
  double time_passed;
  int room_to_be_generated = 50;
  double simulation_timestep = 0.1;
  ewdg::Dungeon d;
  bool simulation_done = false;

protected:
  static void _bind_methods() {
    ClassDB::bind_method(D_METHOD("get_rooms_to_be_generated"),
                         &GDExample::get_rooms_to_be_generated);

    ClassDB::bind_method(
        D_METHOD("set_rooms_to_be_generated", "p_room_to_be_generated"),
        &GDExample::set_rooms_to_be_generated);

    ClassDB::add_property(
        "GDExample", PropertyInfo(Variant::INT, "rooms_to_be_generated"),
        "set_rooms_to_be_generated", "get_rooms_to_be_generated");

    ClassDB::bind_method(D_METHOD("get_simulation_timestep"),
                         &GDExample::get_simulation_timestep);
    ClassDB::bind_method(
        D_METHOD("set_simulation_timestep", "p_simulation_timestep"),
        &GDExample::set_simulation_timestep);
    ClassDB::add_property("GDExample",
                          PropertyInfo(Variant::FLOAT, "simulation_timestep",
                                       PROPERTY_HINT_RANGE, "0.001,1,0.001"),
                          "set_simulation_timestep", "get_simulation_timestep");
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

  void set_simulation_timestep(const double p_simulation_timestep) {
    simulation_timestep = p_simulation_timestep;
  }

  double get_simulation_timestep() const { return simulation_timestep; }
};

} // namespace godot

#endif
