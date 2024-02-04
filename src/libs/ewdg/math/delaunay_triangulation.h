#ifndef DELAUNAY_TRIANGULATION_H_
#define DELAUNAY_TRIANGULATION_H_

#include "math/vector2.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <set>
#include <unordered_map>
#include <vector>

namespace ewdg {
constexpr double eps = 1e-4;

// TODO: Ectract to seperat file
template <typename T> struct DisjointSet {
  std::unordered_map<const T *, const T *> parent;

  void make_set(const T *v) { parent[v] = v; }

  const T *find_set(const T *v) {
    if (v == parent[v]) {
      return v;
    }
    return find_set(parent[v]);
  }

  void union_sets(const T *a, const T *b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
      parent[b] = a;
    }
  }
};

template <typename T> struct Triangle {
  const T *t1;
  const T *t2;
  const T *t3;
  struct Circumcircle {
    Vector2 c;
    double r;
  };
  Circumcircle circumCirc;
  Triangle() : t1(nullptr), t2(nullptr), t3(nullptr) {}
  Triangle(const T *a, const T *b, const T *c) : t1(a), t2(b), t3(c) {
    findCircumCenter(t1->position, t2->position, t3->position);
  }

  bool inCircumcircle(Vector2 v) const {
    double dx = circumCirc.c.x - v.x;
    double dy = circumCirc.c.y - v.y;
    return ((dx * dx + dy * dy) - circumCirc.r) <= eps;
  }

  void findCircumCenter(Vector2 p0, Vector2 p1, Vector2 p2) {
    const auto ax = p1.x - p0.x;
    const auto ay = p1.y - p0.y;
    const auto bx = p2.x - p0.x;
    const auto by = p2.y - p0.y;

    const auto m = p1.x * p1.x - p0.x * p0.x + p1.y * p1.y - p0.y * p0.y;
    const auto u = p2.x * p2.x - p0.x * p0.x + p2.y * p2.y - p0.y * p0.y;
    const auto s = 1. / (2. * (ax * by - ay * bx));

    circumCirc.c = {((p2.y - p0.y) * m + (p0.y - p1.y) * u) * s,
                    ((p0.x - p2.x) * m + (p1.x - p0.x) * u) * s};
    const auto dx = p0.x - circumCirc.c.x;
    const auto dy = p0.y - circumCirc.c.y;
    circumCirc.r = dx * dx + dy * dy;
  }

  bool operator==(const Triangle<T> &other) const {
    return t1->position == other.t1->position &&
           t2->position == other.t2->position &&
           t3->position == other.t3->position;
  }
  bool operator<=(const Triangle<T> &other) const {
    return t1->position == other.t1->position ||
           t1->position == other.t2->position ||
           t1->position == other.t3->position ||
           t2->position == other.t1->position ||
           t2->position == other.t2->position ||
           t2->position == other.t3->position ||
           t3->position == other.t1->position ||
           t3->position == other.t2->position ||
           t3->position == other.t3->position;
  }
  bool operator<=(const Vector2 &other) const {
    return t1->position == other || t2->position == other ||
           t3->position == other;
  }
};

template <typename T> struct Edge {
  T *from;
  T *to;
  double weight;

  Edge(T *f, T *t, double w) : from(f), to(t), weight(w) {}

  bool operator<(const Edge &other) const { return weight < other.weight; }

  bool operator==(const Edge &other) const {
    return (from->position == other.from->position &&
            to->position == other.to->position) ||
           (to->position == other.from->position &&
            from->position == other.to->position);
  }
};

template <typename T> class DelaunayTriangulation {
public:
  DelaunayTriangulation<T>() : triangles(), edges(), superTriangle() {}

  std::set<Edge<T>> edges;

  void brutforce_graf(std::vector<T> &vertices) {
    bruteforceDelaunayEdges(vertices);
  }

  // FIXME: Dose not produce expected results
  void generate_graf(const std::vector<T> &vertices) {
    if (vertices.size() < 3)
      return;

    // Initialize the triangulation with a super-triangle
    initialize_super_triangle(vertices);

    for (const auto &v : vertices) {
      insertVertex(v);
    }

    triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
                                   [&](const Triangle<T> &triangle) {
                                     return triangle <= superTriangle;
                                   }),
                    triangles.end());

    for (const auto &triangle : triangles) {
      edges.emplace(triangle.t1, triangle.t2,
                    distance(triangle.t1, triangle.t2));
      edges.emplace(triangle.t2, triangle.t3,
                    distance(triangle.t2, triangle.t3));
      edges.emplace(triangle.t3, triangle.t1,
                    distance(triangle.t3, triangle.t1));
    }
  }

  std::set<Edge<T>> generate_minimum_spanning_tree() {
    std::set<Edge<T>> minimum_spanning_tree;
    DisjointSet<T> ds;

    // Sort the edges based on their weights
    std::vector<Edge<T>> sorted_edges(edges.begin(), edges.end());
    std::sort(
        sorted_edges.begin(), sorted_edges.end(),
        [](const Edge<T> &a, const Edge<T> &b) { return a.weight < b.weight; });

    for (const auto &edge : edges) {
      ds.make_set(edge.from);
      ds.make_set(edge.to);
    }

    for (const auto &edge : sorted_edges) {
      const T *root_from = ds.find_set(edge.from);
      const T *root_to = ds.find_set(edge.to);
      if (root_from != root_to) {
        minimum_spanning_tree.insert(edge); // Insert directly into set
        ds.union_sets(edge.from, edge.to);
      }
    }
    return minimum_spanning_tree;
  }

private:
  Triangle<T> superTriangle;
  T t1{}, t2{}, t3{};
  std::vector<Triangle<T>> triangles;
  void initialize_super_triangle(const std::vector<T> &vecT) {
    double minx = std::numeric_limits<double>::infinity();
    double miny = std::numeric_limits<double>::infinity();
    double maxx = -std::numeric_limits<double>::infinity();
    double maxy = -std::numeric_limits<double>::infinity();

    for (const auto &t : vecT) {
      const auto vertex = t.position;
      minx = std::min(minx, vertex.x);
      miny = std::min(miny, vertex.y);
      maxx = std::max(maxx, vertex.x);
      maxy = std::max(maxy, vertex.y);
    }

    double dx = (maxx - minx) * 10;
    double dy = (maxy - miny) * 10;

    Vector2 v1(minx - dx, miny - dy * 3);
    Vector2 v2(minx - dx, maxy + dy);
    Vector2 v3(maxx + dx * 3, maxy + dy);

    t1.position = v1;
    t2.position = v2;
    t3.position = v3;
    superTriangle = Triangle<T>(&t1, &t2, &t3);
    std::printf("Super1: %s\n", superTriangle.t1->position.toString());
    std::printf("Super2: %s\n", superTriangle.t2->position.toString());
    std::printf("Super3: %s\n", superTriangle.t3->position.toString());
    triangles.insert(superTriangle);
    // triangles.emplace(&t1, &t2, &t3);
  }

  // TODO: FIXME Edges are crossing eachother dont know why... (BowyerWatson
  // meathod)
  void insertVertex(const T &t) {
    Vector2 pt = t.position;
    std::printf("Inserting vertex: %s\n", t.position.toString());
    std::vector<Edge<T>> tmp_edges;
    std::vector<Triangle<T>> new_triangles;

    for (auto const &triangle : triangles) {
      /* Check if the point is inside the triangle circumcircle. */
      if (triangle.inCircumcircle(pt)) {
        tmp_edges.emplace(triangle.t1, triangle.t2,
                          distance(triangle.t1, triangle.t2));
        tmp_edges.emplace(triangle.t2, triangle.t3,
                          distance(triangle.t2, triangle.t3));
        tmp_edges.emplace(triangle.t3, triangle.t1,
                          distance(triangle.t3, triangle.t1));
      } else {
        new_triangles.insert(triangle);
      }
    }

    removeDuplicates(tmp_edges);

    for (auto const &e : tmp_edges) {
      new_triangles.emplace(e.from, e.to, &t);
    }

    triangles = std::move(new_triangles);
  }

  // TODO: Replace Temporaty fix
  // =======================================================================
  bool inCircle(const Vector2 &a, const Vector2 &b, const Vector2 &c,
                const Vector2 &d) {
    double det = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
    if (std::fabs(det) < 1e-9)
      return false; // Points are collinear, return false

    double cx = ((b.x - a.x) * (b.x + a.x) + (b.y - a.y) * (b.y + a.y)) / 2.0;
    double cy = ((c.x - a.x) * (c.x + a.x) + (c.y - a.y) * (c.y + a.y)) / 2.0;

    double d_inv = 1.0 / det;
    double center_x = (cx * (c.y - a.y) - cy * (b.y - a.y)) * d_inv;
    double center_y = ((b.x - a.x) * cy - (c.x - a.x) * cx) * d_inv;

    double radius_sq =
        std::pow(a.x - center_x, 2) + std::pow(a.y - center_y, 2);
    double distance_sq =
        std::pow(d.x - center_x, 2) + std::pow(d.y - center_y, 2);

    return distance_sq <= radius_sq;
  }

  void bruteforceDelaunayEdges(std::vector<T> &vertices) {
    int n = vertices.size();
    edges.clear();

    for (int i = 0; i < n - 2; ++i) {
      for (int j = i + 1; j < n - 1; ++j) {
        for (int k = j + 1; k < n; ++k) {
          bool valid = true;
          for (int p = 0; p < n; ++p) {
            if (p == i || p == j || p == k)
              continue;
            if (inCircle(vertices[i].position, vertices[j].position,
                         vertices[k].position, vertices[p].position)) {
              valid = false;
              break;
            }
          }
          if (valid) {
            edges.insert(Edge<T>(
                &vertices[std::min(i, j)], &vertices[std::max(i, j)],
                distance(vertices[std::min(i, j)], vertices[std::max(i, j)])));
            edges.insert(Edge<T>(
                &vertices[std::min(j, k)], &vertices[std::max(j, k)],
                distance(vertices[std::min(j, k)], vertices[std::max(j, k)])));
            edges.insert(Edge<T>(
                &vertices[std::min(k, i)], &vertices[std::max(k, i)],
                distance(vertices[std::min(k, i)], vertices[std::max(k, i)])));
          }
        }
      }
    }
  }
  // =======================================================================

  double distance(const T *vertex1, const T *vertex2) {
    return (vertex1->position - vertex2->position).length();
  }

  double distance(const T &vertex1, const T &vertex2) {
    return (vertex1.position - vertex2.position).length();
  }
  void removeDuplicates(std::vector<Edge<T>> &edges) {
    // Sort the edges vector
    std::sort(edges.begin(), edges.end());

    // Use std::unique to move duplicates to the end of the vector
    auto uniqueEnd = std::unique(edges.begin(), edges.end());

    // Erase the duplicates from the vector
    edges.erase(uniqueEnd, edges.end());
  }
};
} // namespace ewdg
namespace std {
template <typename T> struct hash<ewdg::Edge<T>> {
  size_t operator()(const ewdg::Edge<T> &edge) const {
    // Assuming `Edge` has `from`, `to`, and `weight` members
    size_t hashValue = 17;
    hashValue = hashValue * 31 + std::hash<T>()(*edge.from);
    hashValue = hashValue * 31 + std::hash<T>()(*edge.to);
    hashValue = hashValue * 31 + std::hash<double>()(edge.weight);
    return hashValue;
  }
};
} // namespace std
#endif // DELAUNAY_TRIANGULATION_H_
