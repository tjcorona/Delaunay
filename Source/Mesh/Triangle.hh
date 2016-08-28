#ifndef DELAUNAY_MESH_TRIANGLE_HH
#define DELAUNAY_MESH_TRIANGLE_HH

#include <set>

#include "Mesh/Vertex.hh"
#include "Mesh/Edge.hh"

#include "Shape/Triangle.hh"

namespace Delaunay
{
namespace Mesh
{

class Triangle : public Shape::Triangle
{
public:
  Triangle(const Edge& ab,const Edge& bc,const Edge& ac);

  ~Triangle();

  const Edge& AB() const;
  const Edge& BC() const;
  const Edge& AC() const;
  const Vertex& A() const;
  const Vertex& B() const;
  const Vertex& C() const;
};

}
}

#endif
