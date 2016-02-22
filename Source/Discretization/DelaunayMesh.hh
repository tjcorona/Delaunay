#ifndef DELAUNAY_DISCRETIZATION_DELAUNAYMESH_HH
#define DELAUNAY_DISCRETIZATION_DELAUNAYMESH_HH

#include <deque>

#include "Shape/Point.hh"
#include "Mesh.hh"

namespace Delaunay
{
namespace Discretization
{

class DelaunayMesh : public Mesh
{
public:
  typedef Shape::Point Point;
  typedef Shape::Triangle Triangle;

  DelaunayMesh() {}

  void AddPerimeterPoint(const Point& p);
  void AddInteriorPoint(const Point& p);

  bool TestDelaunayCondition(TriangleSet& illegalTriangles) const;

private:
  void ConstructInitialMeshFromPerimeter();
  const Triangle* FindContainingTriangle(const Point& p) const;
  void SplitTriangle(const Triangle* t,const Vertex* v);
  void ExtendMesh(const Vertex* v);
  void LegalizeEdges(const Vertex* v,EdgeSet &edges);
};

}
}

#endif
