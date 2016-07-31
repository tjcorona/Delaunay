#include "Polygon.hh"

#include "Shape/Point.hh"

namespace
{
typedef Delaunay::Shape::PointVector PointVector;

PointVector Sort(const PointVector& points)
{
  PointVector pts;

  unsigned positionOfSmallest = std::distance(points.begin(),
					      std::min_element(points.begin(),
							       points.end()));
  unsigned size = points.size();
  for (unsigned i=0;i<size;i++)
  {
    pts.push_back(std::cref(points[(i+positionOfSmallest)%size]));
  }

  return pts;
}
}

namespace Delaunay
{
namespace Shape
{

Polygon::Polygon(const PointVector& points) : Points(Sort(points))
{
}

bool Polygon::Contains(const Point& p) const
{
  (void)p;
  return false;
}

double Polygon::Distance(const Point& p) const
{
  (void)p;
  return 0.;
}

}
}
