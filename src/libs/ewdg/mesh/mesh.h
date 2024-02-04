#ifndef MESH_H_
#define MESH_H_

#include "quad.h"
#include <vector>

namespace ewdg {

class Mesh3D {
public:
  // Constructor
  Mesh3D() {}

  // Function to push back a quad
  void push_back(Quad const &quad) {
    unsigned int start_index =
        m_vertices.size() / 3; // Each vertex has x, y, z components

    auto verts = quad.getVertices();
    m_vertices.insert(m_vertices.end(), verts.begin(), verts.end());

    auto indices = quad.getIndices();
    for (unsigned int index : indices) {
      m_indices.push_back(index + start_index);
    }

    auto uvs = quad.getUVs();
    m_uvs.insert(m_uvs.end(), uvs.begin(), uvs.end());
  }

  // Getter for vertices
  const std::vector<float> &getVertices() const { return m_vertices; }

  // Getter for indices
  const std::vector<unsigned int> &getIndices() const { return m_indices; }

  // Getter for UVs
  const std::vector<float> &getUVs() const { return m_uvs; }

private:
  std::vector<float> m_vertices;
  std::vector<unsigned int> m_indices;
  std::vector<float> m_uvs;
};

} // namespace ewdg

#endif // MESH_H_
