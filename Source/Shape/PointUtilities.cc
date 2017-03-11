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

#include "Shape/PointUtilities.hh"

#include "Shape/Point.hh"

#include <cmath>

namespace Delaunay
{
namespace Shape
{

int Orientation(const Point& p1, const Point& p2, const Point& p3)
{
  // +1: counterclockwise
  //  0: colinear
  // -1: clockwise
  Point v1 = p2 - p1;
  Point v2 = p3 - p1;

  double cross = Cross(v1,v2);
  if (fabs(cross) < EPSILON)
    return 0;
  else
    return (cross > 0. ? 1 : -1);
}

double Dot(const Point& p1,const Point& p2)
{
  return p1.x*p2.x + p1.y*p2.y;
}

double Cross(const Point& p1,const Point& p2)
{
  return p1.x*p2.y - p1.y*p2.x;
}

double DistanceSquared(const Point& p1, const Point& p2)
{
  return (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y);
}

double Distance(const Point& p1, const Point& p2)
{
  return std::sqrt(DistanceSquared(p1,p2));
}

double Angle(const Point& p1, const Point& p2, const Point& p3)
{
  double a = Distance(p1,p2);
  double b = Distance(p2,p3);
  double c2 = DistanceSquared(p1,p3);

  return (acos((a*a + b*b - c2)/(2.*a*b)) +
          M_PI*(Orientation(p1,p2,p3) == 1));
}

}
}
