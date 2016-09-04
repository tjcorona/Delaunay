#ifndef DELAUNAY_SHAPE_POLYGON_HH
#define DELAUNAY_SHAPE_POLYGON_HH

#include "Shape/Point.hh"

#include <iostream>

namespace Delaunay
{
namespace Shape
{

class Polygon
{
public:

  Polygon(const std::vector<Point>&);
  Polygon(const PointVector&);
  ~Polygon() {}

  void SetPoints(const PointVector& p) { this->Points = p; }
  const PointVector& GetPoints() const { return this->Points; }

  bool   Contains(const Point& p) const;
  double Distance(const Point& p) const;

protected:
  PointVector Points;
};

}
}

#endif
