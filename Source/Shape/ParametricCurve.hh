#ifndef DELAUNAY_SHAPE_PARAMETRICCURVE_HH
#define DELAUNAY_SHAPE_PARAMETRICCURVE_HH

namespace Delaunay
{
namespace Shape
{

class Point;

class ParametricCurve
{
public:
  ParametricCurve(bool c) : Closed(c) {}
  virtual ~ParametricCurve() {}

  virtual const Point operator() (double) const = 0;

  const bool Closed;
};

}
}

#endif
