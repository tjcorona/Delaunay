#ifndef DELAUNAY_MESH_POLYGON_HH
#define DELAUNAY_MESH_POLYGON_HH

#include <vector>

#include "Mesh/Vertex.hh"
#include "Shape/Polygon.hh"

namespace Delaunay
{
namespace Mesh
{

class Polygon : public Shape::Polygon
{
public:
  Polygon(const Shape::PointVector&);
  ~Polygon() {}

  unsigned size() const { return this->Points.size(); }
  const Vertex& operator[](unsigned) const;

private:

};

}
}

#endif
