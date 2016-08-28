#include "Shape/Triangle.hh"
#include "TestFunction.hh"

int UnitTestTriangle(int /*argc*/,char** /*argv*/)
{
  typedef Delaunay::Shape::Point Point;
  typedef Delaunay::Shape::LineSegment LineSegment;
  typedef Delaunay::Shape::Triangle Triangle;

  Point p00(0.,0.);
  Point p01(1.,0.);
  Point p02(0.,1.);
  LineSegment l00(p00,p01);
  LineSegment l01(p01,p02);
  LineSegment l02(p02,p00);
  Triangle t0(l00,l01,l02);

  Point p10(1.,1.);
  Point p11(1.,0.);
  Point p12(0.,1.);
  LineSegment l10(p10,p11);
  LineSegment l11(p11,p12);
  LineSegment l12(p12,p10);
  Triangle t1(l10,l11,l12);

  Triangle t2(l00,l01,l02);

  test(t0 != t1, "Different triangles are reported to be equal.");
  test(t0 == t2, "The same triangles are reported to be unequal.");
  test(t0 < t1, "Unexpected inequality result between different triangles.");

  return 0;
}
