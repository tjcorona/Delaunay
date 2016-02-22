#ifndef DELAUNAY_SHAPE_LINESEGMENT_HH
#define DELAUNAY_SHAPE_LINESEGMENT_HH

#include "Point.hh"

namespace Delaunay
{
namespace Shape
{

class LineSegment
{
public:
  LineSegment(const Point& pp1,const Point& pp2) : p1(pp1), p2(pp2) {}

  const Point p1;
  const Point p2;

  bool Contains(const Point& p) const;

  friend bool operator==(const LineSegment& l1,const LineSegment& l2)
  {
    return l1.p1 == l2.p1 && l1.p2 == l2.p2;
  }

  friend bool operator!=(const LineSegment& l1,const LineSegment& l2)
  {
    return !(l1 == l2);
  }

  friend bool operator<(const LineSegment& l1,const LineSegment& l2)
  {
    return (l1.p1 != l2.p1 ? l1.p1 < l2.p1 : l1.p2 < l2.p2);
  }

  friend bool operator>=(const LineSegment& l1,const LineSegment& l2)
  {
    return !(l1<l2);
  }

  friend bool operator>(const LineSegment& l1,const LineSegment& l2)
  {
    return (l1.p1 != l2.p1 ? l1.p1 > l2.p1 : l1.p2 > l2.p2);
  }

  friend bool operator<=(const LineSegment& l1,const LineSegment& l2)
  {
    return !(l1>l2);
  }

  friend std::ostream& operator<<(std::ostream& s,const LineSegment& l)
  {
    s<<"("<< l.p1<<","<<l.p2<<")";
    return s;
  }
};

}
}

#endif
