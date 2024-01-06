#ifndef PATH_H_
#define PATH_H_

#include <algorithm>
#include <vector>

#include "math/vector2.h"
#include "math/vector3.h"

namespace ewdg {
class Path {
  static constexpr double straight_path_threshhold = 5.0f;

public:
  Vector2 start, end;
  double width, floor_to_ceiling;

  bool straight_path = std::abs(start.x - end.x) > straight_path_threshhold ||
                       std::abs(start.y - end.y) > straight_path_threshhold;

  Path(Vector2 start, Vector2 end, double width = 10,
       double floor_to_ceiling = 3.0f)
      : start(start), end(end), width(width),
        floor_to_ceiling(floor_to_ceiling) {}

  void generate_3d_mesh(std::vector<Vector3> &vertices,
                        std::vector<int32_t> &indices) const {
    double half_width = width / 2;
    size_t baseIndex = vertices.size();
    if (straight_path) {
      Vector2 path_dir = (start - end).normalize();

      auto p_start = start;
      auto p_end = end;
      double cos = Vector2::dot(path_dir, Vector2(1, 0));
      // if (cos < 0) {
      // p_start = end;
      // p_end = start;
      //}

      double x_diff = 0, y_diff = 0;
      if (cos < 0.5 && cos > -0.5) {
        x_diff = half_width;
      } else {
        y_diff = half_width;
      }

      vertices.emplace_back(p_start.x - x_diff, 0,
                            p_start.y - y_diff);                    // Vertex 0
      vertices.emplace_back(p_end.x - x_diff, 0, p_end.y - y_diff); // Vertex 1

      vertices.emplace_back(p_end.x + x_diff, 0, p_end.y + y_diff); // Vertex 2
      vertices.emplace_back(p_start.x + x_diff, 0,
                            p_start.y + y_diff); // Vertex 3

      vertices.emplace_back(p_start.x - x_diff, floor_to_ceiling,
                            p_start.y - y_diff); // Vertex 4
      vertices.emplace_back(p_end.x - x_diff, floor_to_ceiling,
                            p_end.y - y_diff); // Vertex 5

      vertices.emplace_back(p_end.x + x_diff, floor_to_ceiling,
                            p_end.y + y_diff); // Vertex 6
      vertices.emplace_back(p_start.x + x_diff, floor_to_ceiling,
                            p_start.y + y_diff); // Vertex 7

      std::vector<int> localIndices = {
          0, 1, 2, 0, 2, 3, // Floor
          4, 6, 5, 4, 7, 6, // Ceiling
          5, 1, 0, 5, 0, 4, // Wall1
          6, 3, 2, 6, 7, 3  // Wall2
      };

      // Adjust indices and add to the main indices vector
      for (int index : localIndices) {
        indices.push_back(index + baseIndex);
      }

    } else {
    }
  }

}; // namespace ewdg

} // namespace ewdg
#endif // PATH_H_
