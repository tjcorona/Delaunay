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

#include "LineSegment.hh"

#include <algorithm>

namespace
{
  typedef Delaunay::Shape::Point Point;

  const Point& First(const Point& a,const Point& b) { return (a < b ? a : b); }
  const Point& Second(const Point& a,const Point& b) { return (a < b ? b : a); }
}

namespace Delaunay
{
namespace Shape
{
  LineSegment::LineSegment(const Point& a,const Point& b) :
    A(First(a,b)), B(Second(a,b)) {}
}
}
