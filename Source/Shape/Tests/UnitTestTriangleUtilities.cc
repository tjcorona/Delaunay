/******************************************************************************

  This source file is part of the Delaunay project.

  Copyright T.J. Corona

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#include "Shape/Triangle.hh"
#include "Shape/TriangleUtilities.hh"
#include "TestFunction.hh"

int UnitTestTriangleUtilities(int /*argc*/,char** /*argv*/)
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

  Point p(.5,.5);
  LineSegment l0(p10,p);
  Point p1(.75,.75);
  LineSegment l1(p10,p1);
  Point p2(.25,.25);
  LineSegment l2(p00,p2);
  LineSegment l3(p1,p2);

  Point p3(0.,1.5);
  LineSegment l4(p00,p3);

  Triangle t2(l00,l01,l02);

  Point p20 = p00 + p10;
  Point p21 = p01 + p10;
  Point p22 = p02 + p10;
  LineSegment l20(p20,p21);
  LineSegment l21(p21,p22);
  LineSegment l22(p22,p20);
  Triangle t3(l20,l21,l22);

  test(Coincident(t0, Point(0.,.5)) == true,
       "Coincident point on triangle returned unexpected result.");
  test(Coincident(t0, p10) == false,
       "Coincident point not on triangle returned unexpected result.");

  test(std::abs(Area(t0) - .5) < EPSILON,
       "Triangle area returned unexpected result.");

  test(ClosestPoint(t0, p10) == p,
       "Closest point on triangle returned unexpected result.");

  test(std::abs(Distance(t0, p10) - std::sqrt(.5)) < EPSILON,
       "Distance from triangle returned unexpected result.");

  test(Coincident(t0,l0) == true,
       "Coincident line to triangle returned unexpected result.");
  test(Coincident(t0,l1) == false,
       "Non-coincident line to triangle returned unexpected result.");

  test(Contains(t0,l2) == true,
       "Line contained within triangle returned unexpected result.");
  test(Contains(t0,l00) == true,
       "Triangle edge should be contained by the triangle.");
  test(Contains(t0,l3) == false,
       "Line not contained within triangle returned unexpected result.");

  test(Intersect(t0,l2) == false,
       "Line within triangle does not intersect the triangle.");
  test(Intersect(t0,l00) == false,
       "Triangle edge does not intersect the triangle.");
  test(Intersect(t0,l4) == false,
       "Extended triangle edge should not intersect the triangle.");
  test(Intersect(t0,l3) == true,
       "Intersecting line should intersect triangle.");

  test(ClosestPoint(t0,l1) == p,
       "Closest point on triangle to line segment returned unexpected result.");
  test(ClosestPoint(t0,l2) == p00,
       "Closest point on triangle to line segment returned unexpected result.");

  test(std::abs(Distance(t0,l1) - std::sqrt(2.)*.25) < EPSILON,
       "Distance from triangle to line segment returned unexpected result.");
  test(std::abs(Distance(t0,l2)) < EPSILON,
       "Closest point on triangle to line segment returned unexpected result.");

  test(Coincident(t0,t1) == true,
       "Coincident triangles returned unexpected result.");
  std::cout<<"Coincident "<<t0<<" and "<<t3<<" is "<<Coincident(t0,t3)<<std::endl;
  test(Coincident(t0,t3) == false,
       "Non-coincident triangles returned unexpected result.");

  test(std::abs(Distance(t0,t1)) < EPSILON,
       "Coincident triangles returned nonzero distance.");
  test(std::abs(Distance(t0,t3) - 1./std::sqrt(2.)) < EPSILON,
       "Non-coincident triangles returned unexpected distance.");

  return 0;
}
