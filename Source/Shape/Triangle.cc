#include "Triangle.hh"

#include <iostream>

namespace
{
  typedef Delaunay::Shape::Point Point;
  typedef Delaunay::Shape::LineSegment LineSegment;

  const LineSegment& First(const LineSegment& a,
			   const LineSegment& b,
			   const LineSegment& c)
  {
    if (a < b) return (a < c ? a : c);
    return (b < c ? b : c);
  }

  const LineSegment& Second(const LineSegment& a,
			    const LineSegment& b,
			    const LineSegment& c)
  {
    if (a > b)
    {
      if (c > a) return a;
      return (b > c ? b : c);
    }

    if (a > c) return a;
    return (b > c ? c : b);
  }

  const LineSegment& Third(const LineSegment& a,
			   const LineSegment& b,
			   const LineSegment& c)
  {
    if (a > b) return (a > c ? a : c);
    return (b > c ? b : c);
  }

Point ComputeCircumcenter(const LineSegment& ls1, const LineSegment& ls2)
{
  const Point& A = ls1.A;
  const Point& B = ls1.B;
  const Point& C = (ls2.A == A || ls2.A == B ? ls2.B : ls2.A);
  double D = 2.*(A.x*(B.y - C.y) + B.x*(C.y - A.y) + C.x*(A.y - B.y));

  Point U(((A.x*A.x + A.y*A.y)*(B.y-C.y) +
	   (B.x*B.x + B.y*B.y)*(C.y-A.y) +
	   (C.x*C.x + C.y*C.y)*(A.y-B.y))/D,
	  ((A.x*A.x + A.y*A.y)*(C.x-B.x) +
	   (B.x*B.x + B.y*B.y)*(A.x-C.x) +
	   (C.x*C.x + C.y*C.y)*(B.x-A.x))/D);
  return U;
}
}

namespace Delaunay
{
namespace Shape
{

Triangle::Triangle(const LineSegment& ab,
		   const LineSegment& ac,
		   const LineSegment& bc) :
  AB(First(ab,bc,ac)), AC(Second(ab,bc,ac)), BC(Third(ab,bc,ac)),
  circumcenter(ComputeCircumcenter(ab,ac))
{
  this->circumradius = this->circumcenter.Distance(AB.A);
}

bool Triangle::Contains(const Point& p) const
{
  const Point& A = AB.A;
  const Point& B = AB.B;
  const Point& C = AC.B;

  double abx = B.x - A.x;
  double aby = B.y - A.y;
  double acx = C.x - A.x;
  double acy = C.y - A.y;
  double apx = p.x - A.x;
  double apy = p.y - A.y;

  double a = abx/aby;
  double b = acx - a*acy;
  double c = apx - a*apy;

  double t = c/b;
  double s = apy/aby - (acy/aby)*t;

  return (s >= 0.&& t >= 0. && (s + t) <= 1.);
}

double Triangle::Distance(const Point& p) const
{
  (void)p;
  return 0.;
}

}
}
