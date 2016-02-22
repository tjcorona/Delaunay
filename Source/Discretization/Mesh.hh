#ifndef DELAUNAY_DISCRETIZATION_MESH_HH
#define DELAUNAY_DISCRETIZATION_MESH_HH

#include <vector>

#include "Shape/Point.hh"
#include "Shape/Triangle.hh"

namespace Delaunay
{
namespace Discretization
{

class Mesh
{
public:
  typedef Shape::Triangle Triangle;
  typedef Triangle::Edge Edge;
  typedef Triangle::Vertex Vertex;
  typedef std::set<const Triangle*> TriangleSet;
  typedef std::set<const Edge*> EdgeSet;
  typedef std::set<const Vertex*> VertexSet;
  typedef std::vector<const Vertex*> Perimeter;

  virtual ~Mesh();

  const TriangleSet& GetTriangles() const { return fTriangles; }
  const VertexSet& GetVertices() const { return fVertices; }

protected:
  Perimeter fPerimeter;
  TriangleSet fTriangles;
  VertexSet fVertices;
};

}
}

#endif
