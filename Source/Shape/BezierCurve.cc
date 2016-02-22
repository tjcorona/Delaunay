#include "BezierCurve.hh"

#include "Point.hh"

namespace Delaunay
{
namespace Shape
{

const Point BezierCurve::operator() (double t) const
{
  std::vector<Point> v1 = points;
  if (Closed())
    v1.push_back(Point(points.front()));
  std::vector<Point> v2;

  return RecursiveBezier(t,v1,v2);
}

const Point BezierCurve::RecursiveBezier(double t,std::vector<Point>& v1,std::vector<Point>& v2) const
{
  v2.clear();
  for (size_t i=0;i<v1.size()-1;i++)
  {
    v2.push_back(Point((1.-t)*v1[i] + t*v1[i+1]));
  }
  if (v2.size()>1)
    return RecursiveBezier(t,v2,v1);
  else
    return v2[0];
}

std::vector<Point> BezierCurve::C1Continuity() const
{

}

}
}
