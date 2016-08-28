#include "LineSegment.hh"

#include <algorithm>

namespace
{
  typedef Delaunay::Shape::Point Point;

  const Point& First(const Point& a,const Point& b) { return (a < b ? a : b); }
  const Point& Second(const Point& a,const Point& b) { return (a < b ? b : a); }
}

namespace Delaunay
{
namespace Shape
{
  LineSegment::LineSegment(const Point& a,const Point& b) :
    A(First(a,b)), B(Second(a,b)) {}
}
}
