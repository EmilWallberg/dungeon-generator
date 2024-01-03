#include "gdexample.h"
#include "libs/ewdg/ewgd.h"
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>

using namespace godot;

void GDExample::_bind_methods() {}

GDExample::GDExample() {
  // Initialize any variables here.
  time_passed = 0.0;
}

PackedVector3Array convertVector(const std::vector<ewgd::Vector3> &vec) {
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

void GDExample::_ready() {
  auto surface_array = Array();
  surface_array.resize(Mesh::ArrayType::ARRAY_MAX);
  d.generate_rooms(200, 20, 100);
  // d.simulate_rooms(1, 0.99, 0.1);
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
  set_mesh(Ref<Mesh>(_mesh));
}

void GDExample::_process(double delta) {
  time_passed += delta;

  // Vector3 new_position = Vector3(10.0 + (10.0 * sin(time_passed * 2.0)),
  // 10.0 + (10.0 * cos(time_passed * 1.5)), 0.0f);
  // set_position(new_position);
  if (!simulation_done) {
    simulation_done = d.time_step_rooms(1000, 100, 0.8 * delta);

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
    set_mesh(Ref<Mesh>(_mesh));
  }
}
