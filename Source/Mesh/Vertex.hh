#ifndef DELAUNAY_MESH_TRIANGLE_HH
#define DELAUNAY_MESH_TRIANGLE_HH

#include <set>

#include "Point.hh"
#include "LineSegment.hh"

namespace Delaunay
{
namespace Mesh
{

class Triangle;
class Edge;
typedef std::set<const Triangle*> TriangleSet;
typedef std::set<const Edge*> EdgeSet;

class Vertex : public Point
{
public:
  Vertex(const Point& p) : Point(p) {}
  Vertex(double x,double y) : Point(x,y) {}

  mutable TriangleSet triangles;
  mutable EdgeSet edges;
};

}
}

#endif
