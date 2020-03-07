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
#include "Shape/PointUtilities.hh"
#include "TestFunction.hh"

int UnitTestPointUtilities(int /*argc*/,char** /*argv*/)
{
  typedef Delaunay::Shape::Point Point;

  Point p0(0.,0.);
  Point p1(1.,1.);
  Point p2(2.,1.);
  Point p3(1.,2.);
  Point p4(2.,2.);

  test(Orientation(p0,p1,p2) == -1,
       "Clockwise point orientation result expected.");
  test(Orientation(p0,p1,p3) == 1,
       "Counterclockwise point orientation result expected.");
  test(Orientation(p0,p1,p4) == 0,
       "Colinear point orientation result expected.");

  test(std::abs(Dot(p1,p2) - 3.)<EPSILON,
       "Dot product between two points returned unexpected result.");
  test(std::abs(Cross(p2,p1) - 1.)<EPSILON,
       "Cross product magnitude between two points returned unexpected result.");

  test(std::abs(DistanceSquared(p0,p1) - 2.)<EPSILON,
       "Distance squared between two points returned unexpected result.");

  test(std::abs(Distance(p0,p1) - std::sqrt(2.))<EPSILON,
       "Distance squared between two points returned unexpected result.");

  test(std::abs(Angle(p2,p1,p3) - M_PI/2.)<EPSILON,
       "Angle between three points returned unexpected result.");

  test(std::abs(Angle(p3,p1,p2) - 3.*M_PI/2.)<EPSILON,
       "Angle between three points returned unexpected result.");

  return 0;
}
