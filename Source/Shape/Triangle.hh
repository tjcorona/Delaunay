#ifndef DELAUNAY_SHAPE_TRIANGLE_HH
#define DELAUNAY_SHAPE_TRIANGLE_HH

#include <set>

#include "Point.hh"
#include "LineSegment.hh"

namespace Delaunay
{
namespace Shape
{

class Triangle
{
public:
  Triangle(const LineSegment&,const LineSegment&,const LineSegment&);
  ~Triangle() {}

  friend bool operator==(const Triangle& t1,const Triangle& t2)
  {
    return (t1.AB == t2.AB && t1.BC == t2.BC && t1.AC == t2.AC);
  }

  friend bool operator!=(const Triangle& t1,const Triangle& t2)
  {
    return !(t1==t2);
  }

  friend bool operator<(const Triangle& t1,const Triangle& t2)
  {
    return (t1.AB!=t2.AB ? t1.AB<t2.AB :
	    (t1.BC!=t2.BC ? t1.BC<t2.BC : t1.AC<t2.AC));
  }

  friend bool operator>=(const Triangle& t1,const Triangle& t2)
  {
    return !(t1<t2);
  }

  friend bool operator>(const Triangle& t1,const Triangle& t2)
  {
    return (t1.AB!=t2.AB ? t1.AB>t2.AB :
	    (t1.BC!=t2.BC ? t1.BC>t2.BC : t1.AC>t2.AC));
  }

  friend bool operator<=(const Triangle& t1,const Triangle& t2)
  {
    return !(t1>t2);
  }

  friend std::ostream& operator<<(std::ostream& s,const Triangle& t)
  {
    s<<"("<< t.AB<<","<<t.AC<<","<<t.BC<<")";
    return s;
  }

  const LineSegment& AB;
  const LineSegment& AC;
  const LineSegment& BC;

  Point circumcenter;
  double circumradius;

};

}
}

#endif
