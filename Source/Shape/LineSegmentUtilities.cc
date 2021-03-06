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

#include "Shape/LineSegmentUtilities.hh"

#include "Shape/LineSegment.hh"
#include "Shape/Point.hh"
#include "Shape/PointUtilities.hh"

#include <algorithm>
#include <limits>

namespace Delaunay
{
namespace Shape
{
bool Contains(const LineSegment& l, const Point& p)
{
  double len = Length(l);
  if (len < EPSILON)
    return (p == l.A);

  double dot = Dot(p-l.B,l.A - l.B);
  if (std::abs(dot - len*Distance(p,l.B)) > len*EPSILON)
    return false;

  double t = dot/len;
  return (t >= -EPSILON && t <= len + EPSILON);
}

double LengthSquared(const LineSegment& l)
{
  return DistanceSquared(l.A,l.B);
}

double Length(const LineSegment& l)
{
  return Distance(l.A,l.B);
}

Point ClosestPoint(const Point& p, const LineSegment& l)
{
  Point PmA = p - l.A;
  Point BmA = l.B - l.A;
  double t = std::max(0., std::min(1., Dot(PmA,BmA)/LengthSquared(l)));
  return l.A + t*BmA;
}

double Distance(const Point& p, const LineSegment& l)
{
  return Distance(p,ClosestPoint(p,l));
}

namespace
{
// Assuming colinearity, does p lie on l?
bool OnLineSegment(const LineSegment& l, const Point& p)
{
  if (l.A == p || l.B == p)
    return true;
  else if (std::abs(l.A.x - p.x) > EPSILON)
    return (l.A.x < p.x && l.B.x > p.x) || (l.B.x < p.x && l.A.x > p.x);
  else
    return (l.A.y < p.y && l.B.y > p.y) || (l.B.y < p.y && l.A.y > p.y);
}
}

int Contains(const LineSegment& l1, const LineSegment& l2)
{
  if (Orientation(l2.A,l2.B,l1.A) || Orientation(l2.A,l2.B,l1.B) ||
      !(OnLineSegment(l2,l1.A) && OnLineSegment(l2,l1.B)))
    return 0;

  return l1.A == l2.A || l1.B == l2.A || l1.B == l2.A || l1.B == l2.B ? -1 : 1;
}

namespace
{
// Do the two line segments intersect?
// +1: yes
//  0: no
// -1: a point of one line segment lies on the other line segment
int Intersect_(const LineSegment& l1, const LineSegment& l2)
{
  int orientation[4] = {Orientation(l1.A,l1.B,l2.A),
			Orientation(l1.A,l1.B,l2.B),
			Orientation(l2.A,l2.B,l1.A),
			Orientation(l2.A,l2.B,l1.B)};

  if (orientation[0] == 0)
    return (OnLineSegment(l1,l2.A) ? -1 : 0);
  if (orientation[1] == 0)
    return (OnLineSegment(l1,l2.B) ? -1 : 0);
  if (orientation[2] == 0)
    return (OnLineSegment(l2,l1.A) ? -1 : 0);
  if (orientation[3] == 0)
    return (OnLineSegment(l2,l1.B) ? -1 : 0);

  return (orientation[0]*orientation[1] == -1 &&
	  orientation[2]*orientation[3] == -1 ? 1 : 0);
}
}

bool Coincident(const LineSegment& l1, const LineSegment& l2)
{
  return Intersect_(l1,l2) == -1;
}

bool Intersect(const LineSegment& l1, const LineSegment& l2)
{
  return Intersect_(l1,l2) == 1;
}

bool IntersectOrCoincident(const LineSegment& l1, const LineSegment& l2)
{
  return Intersect_(l1,l2) != 0;
}

Point Intersection(const LineSegment& l1, const LineSegment& l2)
{
  double a1 = l1.B.y - l1.A.y;
  double b1 = l1.A.x - l1.B.x;
  double c1 = a1*l1.A.x + b1*l1.A.y;

  double a2 = l2.B.y - l2.A.y;
  double b2 = l2.A.x - l2.B.x;
  double c2 = a2*l2.A.x + b2*l2.A.y;

  double denom = a1*b2 - a2*b1;
  if (std::abs(denom) < EPSILON)
    return Point(std::numeric_limits<double>::quiet_NaN(),
                 std::numeric_limits<double>::quiet_NaN());
  else
  {
    Point p((b2*c1 - b1*c2)/denom, (a1*c2 - a2*c1)/denom);
    if (!Contains(l1,p) || !Contains(l2,p))
      return Point(std::numeric_limits<double>::quiet_NaN(),
                   std::numeric_limits<double>::quiet_NaN());
    else
      return p;
  }
}

Point ClosestPoint(const LineSegment& l1, const LineSegment& l2)
{
  Point l2Bml2A = l2.B - l2.A;
  Point l2Aml1A = l2.A - l1.A;
  Point l1Bml1A = l1.B - l1.A;
  double denom = (l2Bml2A.x*l1Bml1A.y - l1Bml1A.x*l2Bml2A.y);
  if (denom < EPSILON)
  {
    Point p1 = ClosestPoint(l1.A,l2);
    double d1 = Distance(p1,l1.A);
    Point p2 = ClosestPoint(l1.B,l2);
    double d2 = Distance(p2,l1.B);
    Point p3 = ClosestPoint(l2.A,l1);
    double d3 = Distance(p3,l2.A);
    Point p4 = ClosestPoint(l2.B,l1);
    double d4 = Distance(p4,l2.B);

    return ((d1 < d2 && d1 < d3 && d1 < d4 ) ? p1 :
	    (d2 < d3 && d2 < d4) ? p2 :
	    (d3 < d4) ? p3 : p4);
  }
  else
  {
    double s = (l2Bml2A.x*l2Aml1A.y - l2Aml1A.x*l2Bml2A.y)/denom;
    return l1.A + std::max(0.,std::min(1.,s))*l1.B;
  }
}

double Distance(const LineSegment& l1, const LineSegment& l2)
{
  if (Intersect(l1,l2) != 0)
    return 0.;

  Point p1 = ClosestPoint(l1.A,l2);
  Point p2 = ClosestPoint(l1.B,l2);
  Point p3 = ClosestPoint(l2.A,l1);
  Point p4 = ClosestPoint(l2.B,l1);

  return std::min(std::min(Distance(p1,p3),Distance(p1,p4)),
		  std::min(Distance(p2,p3),Distance(p2,p4)));
}

}
}
