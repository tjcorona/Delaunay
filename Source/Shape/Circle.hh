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
  Circle(const Point& c, double r) : center(c), radius(r) {}

  const Point& Center() const { return this->center; }
  double Radius() const { return this->radius; }

private:
  Point center;
  double radius;
};

}
}

#endif
