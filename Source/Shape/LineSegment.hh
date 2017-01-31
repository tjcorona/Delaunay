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

#ifndef DELAUNAY_SHAPE_LINESEGMENT_HH
#define DELAUNAY_SHAPE_LINESEGMENT_HH

#include "Shape/Point.hh"

#include <functional>
#include <vector>

namespace Delaunay
{
namespace Shape
{

class LineSegment
{
public:
  LineSegment(const Point& a,const Point& b);

  friend bool operator==(const LineSegment& l1,const LineSegment& l2)
  {
    return l1.A == l2.A && l1.B == l2.B;
  }

  friend bool operator!=(const LineSegment& l1,const LineSegment& l2)
  {
    return !(l1 == l2);
  }

  friend bool operator<(const LineSegment& l1,const LineSegment& l2)
  {
    return (l1.A != l2.A ? l1.A < l2.A : l1.B < l2.B);
  }

  friend bool operator>=(const LineSegment& l1,const LineSegment& l2)
  {
    return !(l1<l2);
  }

  friend bool operator>(const LineSegment& l1,const LineSegment& l2)
  {
    return (l1.A != l2.A ? l1.A > l2.A : l1.B > l2.B);
  }

  friend bool operator<=(const LineSegment& l1,const LineSegment& l2)
  {
    return !(l1>l2);
  }

  friend std::ostream& operator<<(std::ostream& s,const LineSegment& l)
  {
    s<<"("<< l.A<<","<<l.B<<")";
    return s;
  }

  const Point& A;
  const Point& B;
};

typedef std::vector<std::reference_wrapper<const LineSegment> >
LineSegmentVector;

}
}

#endif
