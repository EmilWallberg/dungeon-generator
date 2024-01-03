#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include "ewgd.h"
#include <godot_cpp/classes/mesh_instance3d.hpp>

namespace godot {

class GDExample : public MeshInstance3D {
  GDCLASS(GDExample, MeshInstance3D);

private:
  double time_passed;
  ewgd::Dungeon d;
  bool simulation_done = false;

protected:
  static void _bind_methods();

public:
  GDExample();
  ~GDExample();

  void _ready() override;
  void _process(double delta) override;
};

} // namespace godot

#endif
