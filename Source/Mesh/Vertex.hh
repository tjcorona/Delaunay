#ifndef DELAUNAY_MESH_VERTEX_HH
#define DELAUNAY_MESH_VERTEX_HH

#include <set>

#include "Shape/Point.hh"

#include "Mesh/EdgeSet.hh"
#include "Mesh/TriangleSet.hh"

namespace Delaunay
{
namespace Mesh
{

class Vertex : public Shape::Point
{
public:
  Vertex(const Shape::Point& p) : Point(p) {}
  Vertex(double x,double y) : Point(x,y) {}
  virtual ~Vertex() {}

  mutable TriangleSet triangles;
  mutable EdgeSet edges;
};

}
}

#endif
