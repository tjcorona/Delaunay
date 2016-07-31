#ifndef DELAUNAY_SHAPE_POLYGON_HH
#define DELAUNAY_SHAPE_POLYGON_HH

#include <functional>
#include <vector>

namespace Delaunay
{
namespace Shape
{
class Point;

typedef std::vector<std::reference_wrapper<const Point> > PointVector;

class Polygon
{
public:

  Polygon(const PointVector&);
  ~Polygon() {}

  bool   Contains(const Point& p) const;
  double Distance(const Point& p) const;

  const PointVector Points;
};

}
}

#endif
