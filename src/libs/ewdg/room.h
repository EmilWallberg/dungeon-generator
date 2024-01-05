#ifndef ROOM_H_
#define ROOM_H_

#include "math/vector2.h"
#include "math/vector3.h"
#include "physics_engine/rect.h"
#include <iostream>
#include <vector>
namespace ewdg {
class Room : public Rect {
public:
  float floor_to_ceiling = 3.0f;

  Room(const Vector2 &position, float width, float height)
      : Rect(width, height, position) {}
  Room() : Rect(10.0f, 10.0f) {}

  void generate_3d_mesh(std::vector<Vector3> &vertices,
                        std::vector<int32_t> &indices) const {
    const std::vector<int> surfaceIndices = {0, 1, 2, 0, 2, 3};

    // Get the corners of the room in 3D space
    Vector3 corners[4] = {
        Vector3(get_topleft_corner().x, 0.0f, get_topleft_corner().y),
        Vector3(get_topright_corner().x, 0.0f, get_topright_corner().y),
        Vector3(get_bottomright_corner().x, 0.0f, get_bottomright_corner().y),
        Vector3(get_bottomleft_corner().x, 0.0f, get_bottomleft_corner().y)};

    // Create vertices for the floor and ceiling
    std::vector<Vector3> floorVertices;
    std::vector<Vector3> ceilingVertices;

    for (int i = 0; i < 4; ++i) {
      floorVertices.push_back(corners[i]);
      ceilingVertices.push_back(corners[i] +
                                Vector3(0.0f, floor_to_ceiling, 0.0f));
    }

    int baseIndex = vertices.size();
    // Push floor vertices and indices to form triangles
    for (const auto &vertex : floorVertices) {
      vertices.push_back(vertex);
    }

    for (const auto &index : surfaceIndices) {
      indices.push_back(baseIndex + index);
    }

    baseIndex = vertices.size();

    // Push ceiling vertices and indices to form triangles
    for (const auto &vertex : ceilingVertices) {
      vertices.push_back(vertex);
    }

    for (auto it = surfaceIndices.rbegin(); it != surfaceIndices.rend(); ++it) {
      indices.push_back(baseIndex + *it);
    }

    for (int i = 0; i < 4; ++i) {
      int next = (i + 1) % 4;
      baseIndex = vertices.size();
      // Wall vertices for each wall
      vertices.push_back(floorVertices[i]);
      vertices.push_back(ceilingVertices[i]);
      vertices.push_back(ceilingVertices[next]);
      vertices.push_back(floorVertices[next]);

      // Push vertices and indices for each wall into the main list
      for (const auto &index : surfaceIndices) {
        indices.push_back(baseIndex + index);
      }
    }
  }
  bool operator==(const Room &other) const {
    return (position == other.position && width == other.width &&
            height == other.height &&
            floor_to_ceiling == other.floor_to_ceiling);
  }
};
} // namespace ewdg
namespace std {
template <> struct hash<ewdg::Room> {
  size_t operator()(const ewdg::Room &room) const {
    size_t hashValue = 17;
    hashValue = hashValue * 31 + std::hash<float>()(room.position.x);
    hashValue = hashValue * 31 + std::hash<float>()(room.position.y);
    hashValue = hashValue * 31 + std::hash<float>()(room.width);
    hashValue = hashValue * 31 + std::hash<float>()(room.height);
    hashValue = hashValue * 31 + std::hash<float>()(room.floor_to_ceiling);
    return hashValue;
  }
};
} // namespace std
#endif // ROOM_H_
