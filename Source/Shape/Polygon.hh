#ifndef DELAUNAY_SHAPE_POLYGON_HH
#define DELAUNAY_SHAPE_POLYGON_HH

#include "Shape/LineSegment.hh"

namespace Delaunay
{
namespace Shape
{

class Point;

class Polygon
{
public:

  Polygon(const LineSegmentVector&);
  ~Polygon() {}

  bool   Contains(const Point& p) const;
  double Distance(const Point& p) const;

  const LineSegmentVector LineSegments;
};

}
}

#endif
