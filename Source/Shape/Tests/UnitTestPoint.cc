#include "Shape/Point.hh"
#include "TestFunction.hh"

int UnitTestPoint(int /*argc*/,char** /*argv*/)
{
  typedef Delaunay::Shape::Point Point;

  Point p0(0.,0.);
  Point p1(1.,1.);
  Point p2(2.,2.);

  test(p0 != p1, "Different points are reported to be equal.");
  test(p0 == Point(0.,0.), "The same points are reported to be unequal.");
  test(p0 < p1, "Unexpected inequality result between different points.");
  test(2*p1 == p2, "Multiplication of points produced incorrect results.");
  test(p1 == p2/2., "Division of points produced incorrect results.");
  test(p2 - p1 == p1, "Subtraction of points produced incorrect results.");
  test(p1 + p1 == p2, "Addition of points produced incorrect results.");

  return 0;
}
