#ifndef DELAUNAY_MESH_TRIANGLE_HH
#define DELAUNAY_MESH_TRIANGLE_HH

#include <set>

#include "Vertex.hh"
#include "Edge.hh"

namespace Delaunay
{
namespace Mesh
{

class Triangle
{
public:
  Triangle(const Edge& ab,const Edge& bc,const Edge& ac) :
    Shape::Triangle(ab,bc,ac)
  {
    A.triangles.insert(this);
    B.triangles.insert(this);
    C.triangles.insert(this);
    AB.triangles.insert(this);
    BC.triangles.insert(this);
    AC.triangles.insert(this);
  }

  ~Triangle()
  {
    A.triangles.erase(this);
    B.triangles.erase(this);
    C.triangles.erase(this);
    AB.triangles.erase(this);
    BC.triangles.erase(this);
    AC.triangles.erase(this);
  }

  const Edge& AB() { return static_cast<const Edge&>(this->AB); }
  const Edge& BC() { return static_cast<const Edge&>(this->BC); }
  const Edge& AC() { return static_cast<const Edge&>(this->AC); }
  const Vertex& A() { return this->AB().A(); }
  const Vertex& B() { return this->AB().B(); }
  const Vertex& C() { return this->BC().B(); }
};

}
}

#endif
