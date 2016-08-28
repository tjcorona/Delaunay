#include "Polygon.hh"

#include "Shape/LineSegment.hh"

namespace
{
typedef Delaunay::Shape::LineSegmentVector LineSegmentVector;

LineSegmentVector Sort(const LineSegmentVector& lineSegments)
{
  LineSegmentVector lines;

  unsigned positionOfSmallest =
    std::distance(lineSegments.begin(), std::min_element(lineSegments.begin(),
							 lineSegments.end()));
  unsigned size = lineSegments.size();
  for (unsigned i=0;i<size;i++)
  {
    lines.push_back(std::cref(lineSegments[(i+positionOfSmallest)%size]));
  }

  return lines;
}
}

namespace Delaunay
{
namespace Shape
{

Polygon::Polygon(const LineSegmentVector& lineSegments) :
  LineSegments(Sort(lineSegments))
{
  if (this->LineSegments[0].get().A == this->LineSegments[1].get().A ||
      this->LineSegments[0].get().A == this->LineSegments[1].get().B)
  {
    this->Points.push_back(std::cref(this->LineSegments[0].get().A));
  }
  else
  {
    this->Points.push_back(std::cref(this->LineSegments[0].get().B));
  }

  for (unsigned i=1;i<this->LineSegments.size();i++)
  {
    if (this->LineSegments[i].get().A == this->Points[i-1])
      this->Points.push_back(std::cref(this->LineSegments[i].get().B));
    else
      this->Points.push_back(std::cref(this->LineSegments[i].get().A));
  }
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
