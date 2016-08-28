#ifndef DELAUNAY_MESH_POLYGON_HH
#define DELAUNAY_MESH_POLYGON_HH

#include <vector>

#include "Shape/Point.hh"
#include "Shape/Polygon.hh"

namespace Delaunay
{
namespace Mesh
{

class Polygon : public Shape::Polygon
{
public:
  Polygon(const std::vector<Point>&);
  Polygon(const Polygon&);
  ~Polygon() {}

private:
};

}
}

#endif
