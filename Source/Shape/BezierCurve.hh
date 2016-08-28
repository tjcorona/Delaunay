#ifndef DELAUNAY_SHAPE_BEZIERCURVE_HH
#define DELAUNAY_SHAPE_BEZIERCURVE_HH

#include <vector>

#include "ParametricCurve.hh"
#include "Point.hh"

namespace Delaunay
{
namespace Shape
{

class BezierCurve : public ParametricCurve
{
public:
  BezierCurve(const PointVector&,bool closed=false);

  const Point operator() (double) const;

protected:
  const Point RecursiveBezier(double,std::vector<Point>&,
			      std::vector<Point>&) const;

  PointVector Points;
};

}
}

#endif
