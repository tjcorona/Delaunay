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

  bool LineSegment::Contains(const Point& p) const
  {
    Point AmB = A - B;
    double l2 = AmB.x*AmB.x + AmB.y*AmB.y;

    if (l2 < EPSILON)
    {
      return (p == A);
    }

    double t = Dot(p-B,AmB)/l2;

    return (t >= 0. && t <= 1.);
  }

  double LineSegment::Distance(const Point& p) const
  {
    Point AmB = A - B;
    double l2 = AmB.x*AmB.x + AmB.y*AmB.y;

    if (l2 < EPSILON)
    {
      return p.Distance(A);
    }

    double t = std::max(0., std::min(1., Dot(p-B,AmB)/l2));
    Point projection = B + t*AmB;
  return p.Distance(projection);
  }
}
}
