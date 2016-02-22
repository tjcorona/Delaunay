#ifndef DELAUNAY_SHAPE_BEZIERCURVE_HH
#define DELAUNAY_SHAPE_BEZIERCURVE_HH

#include <vector>

#include "ParametricCurve.hh"
#include "Point.hh"

namespace Delaunay
{
namespace Shape
{

class Point;

class BezierCurve : public ParametricCurve
{
public:
  const Point operator() (double) const;

  void AddPoint(const Point& p) { points.push_back(Point(p)); }

protected:
  const Point RecursiveBezier(double,std::vector<Point>&,std::vector<Point>&) const;
  std::vector<Point> C1Continuity() const;

  std::vector<Point> points;
};

}
}

#endif
