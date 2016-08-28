#ifndef DELAUNAY_MESH_EDGE_HH
#define DELAUNAY_MESH_EDGE_HH

#include <set>

#include "Shape/LineSegment.hh"
#include "Mesh/Vertex.hh"

namespace Delaunay
{
namespace Mesh
{

class Triangle;
typedef std::set<const Triangle*> TriangleSet;

class Edge : public Shape::LineSegment
{
public:
  Edge(const Vertex& a, const Vertex& b) : LineSegment(a,b)
  {
    this->A().edges.insert(this);
    this->B().edges.insert(this);
  }

  ~Edge()
  {
    this->A().edges.erase(this);
    this->B().edges.erase(this);
  }

  const Vertex& A() const
  {
    return static_cast<const Vertex&>(this->Shape::LineSegment::A);
  }
  const Vertex& B() const
  {
    return static_cast<const Vertex&>(this->Shape::LineSegment::B);
  }

  mutable TriangleSet triangles;
};

}
}

#endif
