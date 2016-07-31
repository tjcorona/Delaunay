#ifndef DELAUNAY_SHAPE_LINESEGMENT_HH
#define DELAUNAY_SHAPE_LINESEGMENT_HH

#include "Shape/Point.hh"

namespace Delaunay
{
namespace Shape
{

class LineSegment
{
public:
  LineSegment(const Point& a,const Point& b);

  friend bool operator==(const LineSegment& l1,const LineSegment& l2)
  {
    return l1.A == l2.A && l1.B == l2.B;
  }

  friend bool operator!=(const LineSegment& l1,const LineSegment& l2)
  {
    return !(l1 == l2);
  }

  friend bool operator<(const LineSegment& l1,const LineSegment& l2)
  {
    return (l1.A != l2.A ? l1.A < l2.A : l1.B < l2.B);
  }

  friend bool operator>=(const LineSegment& l1,const LineSegment& l2)
  {
    return !(l1<l2);
  }

  friend bool operator>(const LineSegment& l1,const LineSegment& l2)
  {
    return (l1.A != l2.A ? l1.A > l2.A : l1.B > l2.B);
  }

  friend bool operator<=(const LineSegment& l1,const LineSegment& l2)
  {
    return !(l1>l2);
  }

  friend std::ostream& operator<<(std::ostream& s,const LineSegment& l)
  {
    s<<"("<< l.A<<","<<l.B<<")";
    return s;
  }

  bool   Contains(const Point& p) const;
  double Distance(const Point& p) const;

  const Point& A;
  const Point& B;
};

}
}

#endif
