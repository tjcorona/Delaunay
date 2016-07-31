#ifndef DELAUNAY_SHAPE_CIRCLE_HH
#define DELAUNAY_SHAPE_CIRCLE_HH

#include "Point.hh"

namespace Delaunay
{
namespace Shape
{

class Circle
{
public:
  Circle(const Point& c, double r) : Center(c), Radius(r) {}

  const Point& Center;
  const double Radius;
};

}
}

#endif
