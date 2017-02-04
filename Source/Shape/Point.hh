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

#ifndef DELAUNAY_SHAPE_POINT_HH
#define DELAUNAY_SHAPE_POINT_HH

#define EPSILON 1.e-12
// #define GRIDRES 14

#include <cmath>
#include <functional>
#include <ostream>
#include <vector>

namespace Delaunay
{
namespace Shape
{

class Point
{
public:
  static double ToGrid(double x)
  {
#ifdef GRIDRES
    return std::floor(x * std::pow(10,GRIDRES) + 0.5)/std::pow(10,GRIDRES);
#else
    return x;
#endif
  }

  Point(double xx,double yy) : x(ToGrid(xx)), y(ToGrid(yy)) {}

  friend bool operator==(const Point& p1,const Point& p2)
  {
    return (std::abs(p1.x - p2.x) < EPSILON && std::abs(p1.y - p2.y) < EPSILON);
  }

  friend bool operator!=(const Point& p1,const Point& p2)
  {
    return !(p1 == p2);
  }

  friend bool operator<(const Point& p1,const Point& p2)
  {
    return (p1.x != p2.x ? p1.x < p2.x : p1.y < p2.y);
  }

  friend bool operator>=(const Point& p1,const Point& p2)
  {
    return !(p1<p2);
  }

  friend bool operator>(const Point& p1,const Point& p2)
  {
    return (p1.x != p2.x ? p1.x > p2.x : p1.y > p2.y);
  }

  friend bool operator<=(const Point& p1,const Point& p2)
  {
    return !(p1>p2);
  }

  friend Point operator*(double d,const Point& p)
  {
    return Point(p.x*d,p.y*d);
  }

  friend Point operator*(const Point& p,double d)
  {
    return operator*(d,p);
  }

  friend Point operator/(const Point& p,double d)
  {
    return Point(p.x/d,p.y/d);
  }

  friend Point operator+(const Point& p1,const Point& p2)
  {
    return Point(p1.x + p2.x,p1.y + p2.y);
  }

  friend Point operator-(const Point& p1,const Point& p2)
  {
    return Point(p1.x - p2.x,p1.y - p2.y);
  }

  friend std::ostream& operator<<(std::ostream& s,const Point& p)
  {
    s<<"("<< p.x<<","<<p.y<<")";
    return s;
  }

  const double x;
  const double y;
};

typedef std::vector<std::reference_wrapper<const Point> > PointVector;

}
}

#endif
