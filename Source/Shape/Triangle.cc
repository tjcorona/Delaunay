#include "Triangle.hh"

namespace Delaunay
{
namespace Shape
{

Triangle::Triangle(const LineSegment& ab,
		   const LineSegment& bc,
		   const LineSegment& ac) :
  AB(ab), BC(bc), AC(ac),
  circumcenter(ComputeCircumcenter()),
  circumradius(circumcenter.Distance(AB.A))
{

}

bool Triangle::Contains(const Point& p) const
{
  const Point& A = AB.A;
  const Point& B = AB.B;
  const Point& C = BC.B;

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

Point Triangle::ComputeCircumcenter() const
{
  const Point& A = AB.A;
  const Point& B = AB.B;
  const Point& C = BC.B;

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
}
