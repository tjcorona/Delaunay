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

#include "Shape/LineSegment.hh"
#include "TestFunction.hh"

int UnitTestLineSegment(int /*argc*/,char** /*argv*/)
{
  typedef Delaunay::Shape::Point Point;
  typedef Delaunay::Shape::LineSegment LineSegment;

  Point p00(0.,0.);
  Point p01(1.,1.);
  LineSegment l0(p00,p01);

  Point p10(1.,0.);
  Point p11(1.,0.);
  LineSegment l1(p10,p11);

  LineSegment l2(p00,p01);

  test(l0 != l1, "Different line segments are reported to be equal.");
  test(l0 == l2, "The same line segments are reported to be unequal.");
  test(l0 < l1, "Unexpected inequality result between different line segments.");

  return 0;
}
