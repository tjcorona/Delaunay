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
  virtual ~ParametricCurve() {}

  virtual const Point operator() (double) const = 0;

  void Closed(bool choice) { closed = choice; }
  bool Closed() const { return closed; }

private:
  bool closed;
};

}
}

#endif
