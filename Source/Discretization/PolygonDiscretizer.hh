#ifndef DELAUNAY_DISCRETIZATION_POLYGONDISCRETIZER_HH
#define DELAUNAY_DISCRETIZATION_POLYGONDISCRETIZER_HH

#include <deque>

#include "Shape/Polygon.hh"
#include "Mesh/Mesher.hh"

namespace Delaunay
{
namespace Discretization
{

class PolygonDiscretizer : public Mesh::Mesher
{
public:
  PolygonDiscretizer() {}

  void Mesh(const Delaunay::Shape::Polygon&, Delaunay::Mesh::Mesh&);

private:
  void EarCuttingMethod(Delaunay::Mesh::Mesh&);

  void AddTriangleToMesh(Mesh::Mesh& mesh,
			 const Mesh::Vertex* a,
			 const Mesh::Vertex* b,
			 const Mesh::Vertex* c);
};

}
}

#endif
