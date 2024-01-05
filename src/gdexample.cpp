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
  time_passed = 0.0;
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
  auto mesh_instace = new MeshInstance3D();
  auto immediate_mesh = new ImmediateMesh();
  auto material = new ORMMaterial3D();

  mesh_instace->set_mesh(immediate_mesh);
  mesh_instace->set_cast_shadows_setting(
      GeometryInstance3D::ShadowCastingSetting::SHADOW_CASTING_SETTING_OFF);

  immediate_mesh->surface_begin(Mesh::PRIMITIVE_LINES, material);
  immediate_mesh->surface_add_vertex(Vector3(pos1.x, pos1.y, pos1.z));
  immediate_mesh->surface_add_vertex(Vector3(pos2.x, pos2.y, pos2.z));
  immediate_mesh->surface_end();

  material->set_shading_mode(
      BaseMaterial3D::ShadingMode::SHADING_MODE_UNSHADED);
  material->set_albedo(c);

  add_child(mesh_instace);
}

void GDExample::_ready() {
  auto surface_array = Array();
  surface_array.resize(Mesh::ArrayType::ARRAY_MAX);
  d.generate_rooms(room_to_be_generated, 20, 100);
  d.simulate_rooms(10, 0.5, simulation_timestep);

  auto room_mesh = d.generate_mesh();
#ifdef DEBUG
  printf("Index array size: %zi\n", room_mesh.second.size());

  // Print Vertex Array
  printf("Vertex Array:\n");
  for (const auto &vec : room_mesh.first) {
    printf("(%f, %f, %f)\n", vec.x, vec.y, vec.z);
  }

  // Convert and Print Index Array
  auto godotVector = convertVector(room_mesh.first);
  printf("Converted Vertex Array:\n");
  for (int i = 0; i < godotVector.size(); ++i) {
    godot::Vector3 godotVec = godotVector[i];
    printf("(%f, %f, %f)\n", godotVec.x, godotVec.y, godotVec.z);
  }

  // Print Index Array
  printf("Index Array:\n");
  for (int i = 0; i < room_mesh.second.size(); ++i) {
    printf("%i\n", room_mesh.second[i]);
  }
#endif

  surface_array[Mesh::ArrayType::ARRAY_VERTEX] = convertVector(room_mesh.first);
  surface_array[Mesh::ArrayType::ARRAY_INDEX] = convertInt(room_mesh.second);
  auto _mesh = new ArrayMesh();
  _mesh->add_surface_from_arrays(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES,
                                 surface_array);
  set_mesh(_mesh);

  d.make_graf_layout(25, 10);
  std::printf("Graf edges: %zi", d.delaunay.edges.size());
  for (ewdg::Edge<ewdg::Room> e : d.dungeon_layout) {
    auto pos1 = ewdg::Vector3(e.from->position.x, 0, e.from->position.y);
    auto pos2 = ewdg::Vector3(e.to->position.x, 0, e.to->position.y);
    line(pos1, pos2);
  }
}

void GDExample::_process(double delta) {
  time_passed += delta;

  if (false && !simulation_done) {
    simulation_done = d.time_step_rooms(10, 1, simulation_timestep);

    auto room_mesh = d.generate_mesh();
#ifdef DEBUG
    printf("Index array size: %zi\n", room_mesh.second.size());

    // Print Vertex Array
    printf("Vertex Array:\n");
    for (const auto &vec : room_mesh.first) {
      printf("(%f, %f, %f)\n", vec.x, vec.y, vec.z);
    }

    // Convert and Print Index Array
    auto godotVector = convertVector(room_mesh.first);
    printf("Converted Vertex Array:\n");
    for (int i = 0; i < godotVector.size(); ++i) {
      godot::Vector3 godotVec = godotVector[i];
      printf("(%f, %f, %f)\n", godotVec.x, godotVec.y, godotVec.z);
    }

    // Print Index Array
    printf("Index Array:\n");
    for (int i = 0; i < room_mesh.second.size(); ++i) {
      printf("%i\n", room_mesh.second[i]);
    }
#endif

    auto surface_array = Array();
    surface_array.resize(Mesh::ArrayType::ARRAY_MAX);
    surface_array[Mesh::ArrayType::ARRAY_VERTEX] =
        convertVector(room_mesh.first);
    surface_array[Mesh::ArrayType::ARRAY_INDEX] = convertInt(room_mesh.second);
    auto _mesh = new ArrayMesh();
    _mesh->add_surface_from_arrays(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES,
                                   surface_array);
    set_mesh(_mesh);
  }
}
