#include "Polygon.hh"

#include "Shape/LineSegment.hh"

namespace
{
typedef Delaunay::Shape::LineSegmentVector LineSegmentVector;

LineSegmentVector Sort(const LineSegmentVector& lineSegments)
{
  LineSegmentVector pts;

  unsigned positionOfSmallest =
    std::distance(lineSegments.begin(), std::min_element(lineSegments.begin(),
							 lineSegments.end()));
  unsigned size = lineSegments.size();
  for (unsigned i=0;i<size;i++)
  {
    pts.push_back(std::cref(lineSegments[(i+positionOfSmallest)%size]));
  }

  return pts;
}
}

namespace Delaunay
{
namespace Shape
{

Polygon::Polygon(const LineSegmentVector& lineSegments) :
  LineSegments(Sort(lineSegments))
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
