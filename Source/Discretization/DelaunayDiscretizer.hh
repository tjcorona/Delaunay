#ifndef DELAUNAY_DISCRETIZATION_DELAUNAYDISCRETIZER_HH
#define DELAUNAY_DISCRETIZATION_DELAUNAYDISCRETIZER_HH

#include <deque>

#include "Shape/Point.hh"
#include "Mesh/Mesher.hh"
#include "Mesh/TriangleSet.hh"

namespace Delaunay
{
namespace Discretization
{

class DelaunayDiscretizer : public Mesh::Mesher
{
public:
  typedef Shape::Point Point;
  typedef Shape::Triangle Triangle;

  DelaunayDiscretizer() {}

  void Mesh(const Delaunay::Shape::Polygon&, Delaunay::Mesh::Mesh&);

  void AddPerimeterPoint(const Point&, Delaunay::Mesh::Mesh&);
  void AddInteriorPoint(const Point&, Delaunay::Mesh::Mesh&);

  bool TestDelaunayCondition(Mesh::TriangleSet& illegalTriangles,
			     const Delaunay::Mesh::Mesh&) const;

private:
  void ConstructInitialMeshFromPerimeter(Delaunay::Mesh::Mesh&);
  const Mesh::Triangle* FindContainingTriangle(const Point& p,
					 Delaunay::Mesh::Mesh&) const;
  void SplitTriangle(const Mesh::Triangle*, const Mesh::Vertex*,
		     Delaunay::Mesh::Mesh&);
  void ExtendMesh(const Mesh::Vertex*, Delaunay::Mesh::Mesh&);
  void LegalizeEdges(const Mesh::Vertex*, std::set<const Mesh::Edge*>&,
		     Delaunay::Mesh::Mesh&);
};

}
}

#endif
