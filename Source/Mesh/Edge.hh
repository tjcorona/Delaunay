#ifndef DELAUNAY_MESH_EDGE_HH
#define DELAUNAY_MESH_EDGE_HH

#include <set>

#include "LineSegment.hh"
#include "Vertex.hh"

namespace Delaunay
{
namespace Mesh
{

class Triangle;
typedef std::set<const Triangle*> TriangleSet;

class Edge : public LineSegment
{
public:
  Edge(const Vertex& a, const Vertex& b) : Shape::LineSegment(a,b)
  {
    this->A().edges.insert(this);
    this->B().edges.insert(this);
  }

  ~Edge()
  {
    this->A().edges.erase(this);
    this->B().edges.erase(this);
  }

  const Vertex& A() { return static_cast<const Vertex&>(A); }
  const Vertex& B() { return static_cast<const Vertex&>(B); }

  mutable TriangleSet triangles;
};

}
}

#endif
