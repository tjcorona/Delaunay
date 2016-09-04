#ifndef DELAUNAY_MESH_MESH_HH
#define DELAUNAY_MESH_MESH_HH

#include <vector>

#include "Mesh/Vertex.hh"
#include "Mesh/Edge.hh"
#include "Mesh/Triangle.hh"
#include "Mesh/Polygon.hh"

namespace Delaunay
{
namespace Mesh
{
class Mesher;

class Mesh
{
public:
  typedef Delaunay::Shape::Polygon Polygon;
  typedef std::set<const Vertex>   VertexSet;
  typedef std::set<const Edge>     EdgeSet;
  typedef std::set<const Triangle> TriangleSet;

  friend class Mesher;

  Mesh() : Perimeter(Shape::PointVector()) {}
  virtual ~Mesh() {}

  const Polygon&     GetPerimeter() const { return this->Perimeter; }
  const VertexSet&   GetVertices()  const { return this->Vertices;  }
  const EdgeSet&     GetEdges()     const { return this->Edges;     }
  const TriangleSet& GetTriangles() const { return this->Triangles; }

protected:
  Polygon     Perimeter;
  VertexSet   Vertices;
  EdgeSet     Edges;
  TriangleSet Triangles;
};

}
}

#endif
