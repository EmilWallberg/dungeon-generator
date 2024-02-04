#ifndef QUAD_H_
#define QUAD_H_
#include <array>
#include <vector>

namespace ewdg {
class Quad {
public:
  // Constructor taking four vertices
  Quad(const std::array<float, 12> &vertices) : vertices_(vertices) {}

  // Function to return vertices
  std::vector<float> getVertices() const {
    return std::vector<float>(vertices_.begin(), vertices_.end());
  }

  // Function to return indices
  std::vector<unsigned int> getIndices() const {
    return {0, 1, 2,
            0, 2, 3}; // Assuming vertices are ordered correctly for a quad
  }

  // Function to return UV coordinates
  std::vector<float> getUVs() const {
    return {0.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f}; // UV coordinates for a square
  }

private:
  std::array<float, 12>
      vertices_; // Assuming each vertex has x, y, z components
};
} // namespace ewdg
#endif // QUAD_H_
