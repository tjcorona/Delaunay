#ifndef DELAUNAY_SHAPE_POLYGON_HH
#define DELAUNAY_SHAPE_POLYGON_HH

#include "Shape/PointVector.hh"

namespace Delaunay
{
namespace Shape
{

class Point;

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
