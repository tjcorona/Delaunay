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

#include "Shape/CircleUtilities.hh"

#include <cmath>
#include <limits>

#include "Shape/Circle.hh"
#include "Shape/LineSegment.hh"
#include "Shape/LineSegmentUtilities.hh"
#include "Shape/Point.hh"
#include "Shape/PointUtilities.hh"
#include "Shape/Triangle.hh"

namespace Delaunay
{
namespace Shape
{
bool Contains(const Circle& c, const Point& p)
{
  return Distance(c.Center,p) < c.Radius;
}

bool Intersect(const Circle& c, const LineSegment& l)
{
  double D = Cross(l.A - c.Center, l.B - c.Center);
  double dr2 = LengthSquared(l);

  return c.Radius*c.Radius*dr2 > D*D;
}

bool Intersect(const LineSegment& l, const Circle& c)
{
  return Intersect(c,l);
}

bool Intersect(const Circle& c, const Triangle& t)
{
  return Intersect(c,t.AB) || Intersect(c,t.BC) || Intersect(c,t.AC);
}

bool Intersect(const Triangle& t, const Circle& c)
{
  return Intersect(c, t);
}

namespace
{
double sign(double d)
{
  return static_cast<double>((0. < d) - (d < 0.));
}
}

std::tuple<unsigned, Point, Point> Intersection(const Circle& c,
                                                const LineSegment& l)
{
  return Intersection(l,c);
}

std::tuple<unsigned, Point, Point> Intersection(const LineSegment& l,
                                                const Circle& c)
{
  double D = Cross(l.A - c.Center, l.B - c.Center);
  double dr2 = LengthSquared(l);

  double Delta = c.Radius*c.Radius*dr2 - D*D;

  if (Delta < -EPSILON/2.)
    return std::make_tuple(0,
                           Point(std::numeric_limits<double>::quiet_NaN(),
                                 std::numeric_limits<double>::quiet_NaN()),
                           Point(std::numeric_limits<double>::quiet_NaN(),
                                 std::numeric_limits<double>::quiet_NaN()));

  Shape::Point d = l.B - l.A;

  double a = D/dr2;

  if (std::abs(Delta) <= EPSILON)
  {
    double x = a*d.y;
    double y = -a*d.x;
    return std::make_tuple(1, Point(x,y) + c.Center, Point(x,y) + c.Center);
  }

  double b = std::sqrt(Delta)/dr2;

  double x1 = a*d.y + sign(d.y)*d.x*b;
  double y1 = -a*d.x + std::abs(d.y)*b;

  double x2 = a*d.y - sign(d.y)*d.x*b;
  double y2 = -a*d.x - std::abs(d.y)*b;

  return std::make_tuple(2, Point(x1,y1) + c.Center, Point(x2,y2) + c.Center);
}

}
}
