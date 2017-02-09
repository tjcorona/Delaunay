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

#ifndef DELAUNAY_SHAPE_TRIANGLE_HH
#define DELAUNAY_SHAPE_TRIANGLE_HH

#include <set>

#include "Point.hh"
#include "LineSegment.hh"
#include "Circle.hh"

namespace Delaunay
{
namespace Shape
{

class Triangle
{
public:
  Triangle(const LineSegment&,const LineSegment&,const LineSegment&);
  ~Triangle() {}

  friend bool operator==(const Triangle& t1,const Triangle& t2)
  {
    return (t1.AB == t2.AB && t1.BC == t2.BC && t1.AC == t2.AC);
  }

  friend bool operator!=(const Triangle& t1,const Triangle& t2)
  {
    return !(t1==t2);
  }

  friend bool operator<(const Triangle& t1,const Triangle& t2)
  {
    return (t1.AB!=t2.AB ? t1.AB<t2.AB :
	    (t1.BC!=t2.BC ? t1.BC<t2.BC : t1.AC<t2.AC));
  }

  friend bool operator>=(const Triangle& t1,const Triangle& t2)
  {
    return !(t1<t2);
  }

  friend bool operator>(const Triangle& t1,const Triangle& t2)
  {
    return (t1.AB!=t2.AB ? t1.AB>t2.AB :
	    (t1.BC!=t2.BC ? t1.BC>t2.BC : t1.AC>t2.AC));
  }

  friend bool operator<=(const Triangle& t1,const Triangle& t2)
  {
    return !(t1>t2);
  }

  friend std::ostream& operator<<(std::ostream& s,const Triangle& t)
  {
    s<<"("<< t.AB<<","<<t.AC<<","<<t.BC<<")";
    return s;
  }

  const LineSegment& AB;
  const LineSegment& AC;
  const LineSegment& BC;

  const Circle circumcircle;
};

}
}

#endif
