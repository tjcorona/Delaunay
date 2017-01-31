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

#include "Shape/Point.hh"
#include "Shape/LineSegment.hh"
#include "Shape/LineSegmentUtilities.hh"
#include "TestFunction.hh"

int UnitTestLineSegmentUtilities(int /*argc*/,char** /*argv*/)
{
  typedef Delaunay::Shape::Point Point;
  typedef Delaunay::Shape::LineSegment LineSegment;

  Point p00(0.,0.);
  Point p01(1.,1.);
  LineSegment l0(p00,p01);

  Point p10(1.,0.);
  Point p11(0.,1.);
  LineSegment l1(p10,p11);

  LineSegment l2(p00,p01);

  Point p2(.5,.5);
  LineSegment l3(p00,p2);

  LineSegment l4(p00,p10);

  LineSegment l5(p10, p01);

  test(Contains(l0,p2) == true,
       "Point should be contained by this line segment.");
  test(Contains(l0,p10) == false,
       "Point should not be contained by this line segment.");

  test(std::abs(LengthSquared(l1)-2.) < EPSILON,
       "Length squared returned incorrect value.");
  test(std::abs(Length(l0)-std::sqrt(2.)) < EPSILON,
       "Length returned incorrect value.");

  test(ClosestPoint(l1,p00) == p2,
       "Closest point returned incorrect point.");
  test(std::abs(Distance(l1,p00) - std::sqrt(.5)) < EPSILON,
       "Distance returned incorrect value.");

  test(Coincident(l1,l3) == true,
       "Coincident lines reported as not coincident.");
  test(Coincident(l0,l4) == true,
       "Coincident lines reported as not coincident.");
  test(Coincident(l0,l3) == true,
       "Coincident lines reported as not coincident.");
  test(Coincident(l0,l1) == false,
       "Non-coincident lines reported as coincident.");

  test(Intersect(l0,l1) == true,
       "Intersecting lines reported as not intersecting.");
  test(Intersect(l0,l3) == false,
       "Non-Intersecting lines reported as intersecting.");
  test(Intersect(l1,l3) == false,
       "Non-intersecting lines reported as intersecting.");

  test(ClosestPoint(l3,l1) == p2,
       "Closest point between two lines returned unexpected result.");

  test(std::abs(Distance(l3,l5) - .5) < EPSILON,
       "Distance between two lines returned unexpected result.");

  return 0;
}
