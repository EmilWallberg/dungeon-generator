#include "gdexample.h"
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/immediate_mesh.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/orm_material3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>

using namespace godot;

GDExample::GDExample() {
  // Initialize any variables here.
  timer = 10;
}

PackedVector3Array convertVector(const std::vector<ewdg::Vector3> &vec) {
  PackedVector3Array packed_array;
  packed_array.resize(vec.size());

  for (int i = 0; i < vec.size(); ++i) {
    packed_array.set(i, Vector3(vec[i].x, vec[i].y, vec[i].z));
  }

  return packed_array;
}

PackedInt32Array convertInt(const std::vector<int32_t> &vec) {
  PackedInt32Array packed_array;
  packed_array.resize(vec.size());

  for (int i = 0; i < vec.size(); ++i) {
    packed_array.set(i, vec[i]);
  }

  return packed_array;
}

GDExample::~GDExample() {
  // Add your cleanup here.
}

void GDExample::line(ewdg::Vector3 pos1, ewdg::Vector3 pos2, Color c) {
  auto mesh_instance = new MeshInstance3D();
  auto immediate_mesh = new ImmediateMesh();
  auto material = new ORMMaterial3D();

  mesh_instance->set_mesh(immediate_mesh);
  mesh_instance->set_cast_shadows_setting(
      GeometryInstance3D::ShadowCastingSetting::SHADOW_CASTING_SETTING_OFF);

  immediate_mesh->surface_begin(Mesh::PRIMITIVE_LINES, material);
  immediate_mesh->surface_add_vertex(Vector3(pos1.x, pos1.y, pos1.z));
  immediate_mesh->surface_add_vertex(Vector3(pos2.x, pos2.y, pos2.z));
  immediate_mesh->surface_end();

  material->set_shading_mode(
      BaseMaterial3D::ShadingMode::SHADING_MODE_UNSHADED);
  material->set_albedo(c);

  add_child(mesh_instance);

  mesh_instances.push_back(mesh_instance);
}

void GDExample::_ready() {
  auto surface_array = Array();
  surface_array.resize(Mesh::ArrayType::ARRAY_MAX);
  d.generate_rooms(room_to_be_generated, min_max_room_width.x,
                   min_max_room_width.y);
  // d.simulate_rooms(repultion_force, friction_force, simulation_timestep);
  //
  // d.make_graf_layout(25, 10);
  // std::printf("Graf edges: %zi", d.delaunay.edges.size());
  // for (ewdg::Edge<ewdg::Room> e : d.dungeon_layout) {
  // auto pos1 = ewdg::Vector3(e.from->position.x, 0, e.from->position.y);
  // auto pos2 = ewdg::Vector3(e.to->position.x, 0, e.to->position.y);
  // line(pos1, pos2);
  //}
  //
  // d.generate_paths();

  auto room_mesh = d.generate_mesh(true);

  surface_array[Mesh::ArrayType::ARRAY_VERTEX] = convertVector(room_mesh.first);
  surface_array[Mesh::ArrayType::ARRAY_INDEX] = convertInt(room_mesh.second);
  auto _mesh = new ArrayMesh();
  _mesh->add_surface_from_arrays(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES,
                                 surface_array);
  set_mesh(_mesh);
}

void GDExample::_process(double delta) {
  if (dungeon_done)
    return;

  timer -= delta;
  if (!simulation_done) {
    simulation_done =
        d.time_step_rooms(repultion_force, friction_force, simulation_timestep);

    auto room_mesh = d.generate_mesh(false);
    auto surface_array = Array();
    surface_array.resize(Mesh::ArrayType::ARRAY_MAX);
    surface_array[Mesh::ArrayType::ARRAY_VERTEX] =
        convertVector(room_mesh.first);
    surface_array[Mesh::ArrayType::ARRAY_INDEX] = convertInt(room_mesh.second);
    auto _mesh = new ArrayMesh();
    _mesh->add_surface_from_arrays(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES,
                                   surface_array);
    set_mesh(_mesh);
  } else if (!graf_done) {
    d.make_graf_layout(25, 10);
    std::printf("Graf edges: %zi", d.delaunay.edges.size());
    for (ewdg::Edge<ewdg::Room> e : d.dungeon_layout) {
      auto pos1 = ewdg::Vector3(e.from->position.x, 0, e.from->position.y);
      auto pos2 = ewdg::Vector3(e.to->position.x, 0, e.to->position.y);
      line(pos1, pos2);
    }

    d.generate_paths();
    graf_done = true;
    timer = 2;
  } else if (!dungeon_done && timer < 0) {
    auto room_mesh = d.generate_mesh(true);
    auto surface_array = Array();
    surface_array.resize(Mesh::ArrayType::ARRAY_MAX);
    surface_array[Mesh::ArrayType::ARRAY_VERTEX] =
        convertVector(room_mesh.first);
    surface_array[Mesh::ArrayType::ARRAY_INDEX] = convertInt(room_mesh.second);
    auto _mesh = new ArrayMesh();
    _mesh->add_surface_from_arrays(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES,
                                   surface_array);
    set_mesh(_mesh);
    for (auto &l : mesh_instances) {
      remove_child(l);
      delete l;
    }

    mesh_instances.clear();
    dungeon_done = true;
  }
}
