#ifndef DELAUNAY_TRIANGULATION_H_
#define DELAUNAY_TRIANGULATION_H_
#include "math/vector2.h"
#include "math/vector3.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ewgd {
// Type trait to check if T has a member named 'position' of type Vector2 or
// Vector3
template <typename T> class has_position {
  template <typename C>
  static auto test(int) -> std::enable_if_t<
      std::is_same<decltype(C::position), Vector2>::value ||
          std::is_same<decltype(C::position), Vector3>::value,
      std::true_type>;

  template <typename C> static std::__false_type test(...);

public:
  static constexpr bool value =
      std::is_same<decltype(test<T>(0)), std::__true_type>::value;
};

template <typename T, typename = std::enable_if_t<has_position<T>::value>>
class DelaunayTriangulation {
public:
  std::unordered_map<T *, std::unordered_set<T *>> adjacency_list;

  void generateGraph(std::vector<T> &vertices) {
    T superTriangle;

    for (auto &v : vertices) {
      superTriangle.position += v.position;
    }
    superTriangle.position /= vertices.size(); // Calculating centroid

    for (auto &v : vertices) {
      v.position *= 1000.0; // To make superTriangle extremely far
      addConnections(superTriangle, v, v);
    }

    for (auto &v : vertices) {
      for (auto &other : vertices) {
        if (&v == &other)
          continue;
        for (auto &third : vertices) {
          if (&v == &third || &other == &third)
            continue;

          if (formsCircumscribedCircle(v, other, third)) {
            addConnections(v, other, third);
          }
        }
      }
    }

    removeSuperTriangleConnections(vertices, superTriangle);
  }

private:
  double calculateDistance(const T &a, const T &b) {
    return (a.position - b.position).length();
  }

  bool formsCircumscribedCircle(const T &a, const T &b, const T &c) {
    double radius = calculateDistance(a, b);
    radius = std::max(radius, calculateDistance(b, c));
    radius = std::max(radius, calculateDistance(c, a));

    auto center =
        (a.position + b.position + c.position) / 3.0; // Calculating center

    return calculateDistance(center, a) <= radius;
  }

  void addConnections(T &t1, T &t2, T &t3) {
    adjacency_list[&t1].insert(&t2);
    adjacency_list[&t1].insert(&t3);
    adjacency_list[&t2].insert(&t1);
    adjacency_list[&t2].insert(&t3);
    adjacency_list[&t3].insert(&t1);
    adjacency_list[&t3].insert(&t2);
  }

  void removeSuperTriangleConnections(const std::vector<T> &vertices,
                                      const T &superTriangle) {
    for (auto &connection : adjacency_list[&superTriangle]) {
      if (connection != &superTriangle) {
        adjacency_list[connection].erase(&superTriangle);
      }
    }
  }
};
} // namespace ewgd
#endif // DELAUNAY_TRIANGULATION_H_
