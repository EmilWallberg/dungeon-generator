#ifndef ROOM_H_
#define ROOM_H_

#include "math/vector2.h"
#include "math/vector3.h"
#include <vector>
namespace ewgd {
class Room {
public:
  Vector2 center;
  float width;
  float height;
  float floor_to_ceiling = 3.0f;

  Room(const Vector2 &c, float w, float h) : center(c), width(w), height(h) {}

  // Functions to get room corners based on center, width, and height
  Vector2 get_topleft_corner() const {
    return center + Vector2(-width / 2, -height / 2);
  }

  Vector2 get_topright_corner() const {
    return center + Vector2(width / 2, -height / 2);
  }

  Vector2 get_bottomright_corner() const {
    return center + Vector2(width / 2, height / 2);
  }

  Vector2 get_bottomleft_corner() const {
    return center + Vector2(-width / 2, height / 2);
  }

  std::vector<Vector3> generate_3d_mesh() const {
    std::vector<Vector3> vertices;

    // Get the corners of the room in 3D space
    Vector3 tl = Vector3(get_topleft_corner().x, 0.0f, get_topleft_corner().y);
    Vector3 tr =
        Vector3(get_topright_corner().x, 0.0f, get_topright_corner().y);
    Vector3 br =
        Vector3(get_bottomright_corner().x, 0.0f, get_bottomright_corner().y);
    Vector3 bl =
        Vector3(get_bottomleft_corner().x, 0.0f, get_bottomleft_corner().y);

    // Create vertices for the floor and ceiling
    Vector3 floor_tl = tl;
    Vector3 floor_tr = tr;
    Vector3 floor_br = br;
    Vector3 floor_bl = bl;

    Vector3 ceiling_tl = tl + Vector3(0.0f, floor_to_ceiling, 0.0f);
    Vector3 ceiling_tr = tr + Vector3(0.0f, floor_to_ceiling, 0.0f);
    Vector3 ceiling_br = br + Vector3(0.0f, floor_to_ceiling, 0.0f);
    Vector3 ceiling_bl = bl + Vector3(0.0f, floor_to_ceiling, 0.0f);

    // Construct the walls of the room using triangles
    // Floor
    vertices.push_back(floor_tl);
    vertices.push_back(floor_tr);
    vertices.push_back(floor_br);

    vertices.push_back(floor_tl);
    vertices.push_back(floor_br);
    vertices.push_back(floor_bl);

    // Ceiling
    vertices.push_back(ceiling_tl);
    vertices.push_back(ceiling_br);
    vertices.push_back(ceiling_tr);

    vertices.push_back(ceiling_tl);
    vertices.push_back(ceiling_bl);
    vertices.push_back(ceiling_br);

    // Walls (triangulated)
    // Wall 1
    vertices.push_back(floor_tl);
    vertices.push_back(ceiling_tl);
    vertices.push_back(ceiling_bl);

    vertices.push_back(floor_tl);
    vertices.push_back(ceiling_bl);
    vertices.push_back(floor_bl);

    // Wall 2
    vertices.push_back(floor_tr);
    vertices.push_back(ceiling_tr);
    vertices.push_back(ceiling_tl);

    vertices.push_back(floor_tr);
    vertices.push_back(ceiling_tl);
    vertices.push_back(floor_tl);

    // Wall 3
    vertices.push_back(floor_br);
    vertices.push_back(ceiling_br);
    vertices.push_back(ceiling_tr);

    vertices.push_back(floor_br);
    vertices.push_back(ceiling_tr);
    vertices.push_back(floor_tr);

    // Wall 4
    vertices.push_back(floor_bl);
    vertices.push_back(ceiling_bl);
    vertices.push_back(ceiling_br);

    vertices.push_back(floor_bl);
    vertices.push_back(ceiling_br);
    vertices.push_back(floor_br);

    return vertices;
  }
};
} // namespace ewgd
#endif // ROOM_H_
