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

#ifndef DELAUNAY_SHAPE_CIRCLE_HH
#define DELAUNAY_SHAPE_CIRCLE_HH

#include "Shape/Export.hh"

#include "Shape/Point.hh"
#include "Shape/PointUtilities.hh"

namespace Delaunay
{
namespace Shape
{

class DELAUNAYSHAPE_EXPORT Circle
{
public:
  Circle(const Point& c, double r) : Center(c), Radius(r) {}
  Circle(const Point& c, const Point& p) : Center(c), Radius(Distance(c,p)) {}

  friend std::ostream& operator<<(std::ostream& s,const Circle& c)
  {
    s<<c.Center<<", "<<c.Radius;
    return s;
  }

  const Point Center;
  const double Radius;
};

}
}

#endif
