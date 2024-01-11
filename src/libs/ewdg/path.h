#ifndef PATH_H_
#define PATH_H_

#include <algorithm>
#include <vector>

#include "math/vector2.h"
#include "math/vector3.h"
#include "room.h"

namespace ewdg {
class Path {
public:
  Vector2 start, end, intersektion;
  double width, floor_to_ceiling;

  bool straight_path;

  Path(Room &r1, Room &r2, double width = 2, double floor_to_ceiling = 3)
      : width(width), floor_to_ceiling(floor_to_ceiling) {

    double overlap_x =
        std::min(r1.position.x + r1.width / 2, r2.position.x + r2.width / 2) -
        std::max(r1.position.x - r1.width / 2, r2.position.x - r2.width / 2);
    double overlap_y =
        std::min(r1.position.y + r1.height / 2, r2.position.y + r2.height / 2) -
        std::max(r1.position.y - r1.height / 2, r2.position.y - r2.height / 2);

    start = r1.position;
    end = r2.position;
    straight_path = false;

    if (overlap_x >= width) {
      straight_path = true;
      start.x =
          std::max(r1.position.x - r1.width / 2, r2.position.x - r2.width / 2) +
          (overlap_x - width) / 2;
      end.x = start.x;

      if (r1.position.y < r2.position.y) {
        start.y += r1.height / 2;
        end.y -= r2.height / 2;
      } else {
        start.y -= r1.height / 2;
        end.y += r2.height / 2;
      }
    } else if (overlap_y >= width) {
      straight_path = true;
      start.y = std::max(r1.position.y - r1.height / 2,
                         r2.position.y - r2.height / 2) +
                (overlap_y - width) / 2;
      end.y = start.y;

      if (r1.position.x < r2.position.x) {
        start.x += r1.width / 2;
        end.x -= r2.width / 2;
      } else {
        start.x -= r1.width / 2;
        end.x += r2.width / 2;
      }
    } else {
      if (r1.position.x < r2.position.x) {
        start.x += r1.width / 2;
      } else {
        start.x -= r1.width / 2;
      }
      if (r1.position.y < r2.position.y) {
        end.y -= r2.height / 2;
      } else {
        end.y += r2.height / 2;
      }
      intersektion = {end.x, start.y};
    }
    r1.entrance_points.push_back(start);
    r2.entrance_points.push_back(end);
    r1.entrance_width = r2.entrance_width = width;
  }

  // TODO FIXME: Check for intersections with rooms and other paths and do some
  // form of union operation.
  void generate_3d_mesh(std::vector<Vector3> &vertices,
                        std::vector<int32_t> &indices) const {
    double half_width = width / 2;

    auto mesh_from_corners = [&](const Vector2 &bl, const Vector2 &br,
                                 const Vector2 &tl, const Vector2 &tr,
                                 const size_t base_index) {
      for (bool top : {false, true}) {
        auto addVertex = [&](const Vector2 &pos) {
          vertices.emplace_back(pos.x, top ? floor_to_ceiling : 0, pos.y);
        };
        addVertex(bl); // Vertex 0
        addVertex(tr); // Vertex 1
        addVertex(br); // Vertex 2
        addVertex(tl); // Vertex 3
      }
      for (int index : {
               1, 2, 0, 3, 1, 0, // Floor
               4, 6, 5, 4, 5, 7, // Ceiling
               2, 1, 5, 5, 6, 2, // Wall1
               7, 3, 0, 0, 4, 7  // Wall2
           }) {
        indices.push_back(index + base_index);
      }
    };

    if (straight_path) {
      Vector2 path_dir = (end - start).normalize();
      Vector2 perpendicular_dir = path_dir.perpendicular();

      Vector2 bottom_left = start - perpendicular_dir * half_width;
      Vector2 bottom_right = start + perpendicular_dir * half_width;
      Vector2 top_left = end - perpendicular_dir * half_width;
      Vector2 top_right = end + perpendicular_dir * half_width;

      mesh_from_corners(bottom_left, bottom_right, top_left, top_right,
                        vertices.size());
    } else {

      // TODO: Clean up dublicated code

      // First segment
      Vector2 path_dir = (intersektion - start).normalize();
      Vector2 perpendicular_dir = path_dir.perpendicular();

      double x_diff = half_width * path_dir.x;
      double y_diff = half_width * path_dir.y;

      Vector2 bottom_left = start - perpendicular_dir * half_width;
      Vector2 bottom_right = start + perpendicular_dir * half_width;

      Vector2 intersektion_offset =
          path_dir *
          (path_dir.cross(end - start) < 0 ? half_width : -half_width);

      Vector2 top_left =
          intersektion - perpendicular_dir * half_width - intersektion_offset;
      Vector2 top_right =
          intersektion + perpendicular_dir * half_width + intersektion_offset;

      mesh_from_corners(bottom_left, bottom_right, top_left, top_right,
                        vertices.size());

      // Secound segment
      path_dir = (intersektion - end).normalize();
      perpendicular_dir = path_dir.perpendicular();

      x_diff = half_width * path_dir.x;
      y_diff = half_width * path_dir.y;
      bottom_left = end - perpendicular_dir * half_width;
      bottom_right = end + perpendicular_dir * half_width;
      intersektion_offset =
          path_dir *
          (path_dir.cross(start - end) < 0 ? half_width : -half_width);

      top_left =
          intersektion - perpendicular_dir * half_width - intersektion_offset;
      top_right =
          intersektion + perpendicular_dir * half_width + intersektion_offset;

      mesh_from_corners(bottom_left, bottom_right, top_left, top_right,
                        vertices.size());
    }
  }
}; // namespace ewdg

} // namespace ewdg
#endif // PATH_H_
