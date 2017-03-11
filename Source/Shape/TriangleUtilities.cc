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

#include "Shape/TriangleUtilities.hh"

#include "Shape/LineSegment.hh"
#include "Shape/LineSegmentUtilities.hh"
#include "Shape/Point.hh"
#include "Shape/PointUtilities.hh"
#include "Shape/Triangle.hh"

#include <utility>

namespace Delaunay
{
namespace Shape
{

namespace
{
// return the parametric coordinates of a point wrt a triangle
std::pair<double,double> ParametricCoordinates(const Point& A,
					       const Point& B,
					       const Point& C,
					       const Point& p)
{
  Point BmA = B - A;
  Point CmA = C - A;
  Point pmA = p - A;

  double a = BmA.x/BmA.y;
  double b = CmA.x - a*CmA.y;
  double c = pmA.x - a*pmA.y;

  double t = c/b;
  double s = pmA.y/BmA.y - (CmA.y/BmA.y)*t;
  return std::make_pair(s,t);
}

std::pair<double,double> ParametricCoordinates(const Triangle& tri,
					       const Point& p)
{
  // Avoid a divide-by-zero in the above function
  return tri.AB.A.y != tri.AB.B.y ?
    ParametricCoordinates(tri.AB.A,tri.AB.B,tri.AC.B,p) :
    ParametricCoordinates(tri.AC.B,tri.AB.A,tri.AB.B,p);
}
}

bool Coincident(const Triangle& tri, const Point& p)
{
  std::pair<double,double> st = ParametricCoordinates(tri,p);
  const double& s = st.first;
  const double& t = st.second;

  return ((std::abs(s) < EPSILON && t >= 0. && t <= 1.) ||
	  (std::abs(t) < EPSILON && s >= 0. && s <= 1.) ||
	  (s >= 0.&& t >= 0. && std::abs(s + t - 1.) < EPSILON));
}

bool Contains(const Triangle& tri, const Point& p)
{
  std::pair<double,double> st = ParametricCoordinates(tri,p);
  const double& s = st.first;
  const double& t = st.second;

  return (s >= -EPSILON && t >= -EPSILON && (s + t) <= 1. ? true : false);
}

double Area(const Triangle& t)
{
  const Point& A = t.AB.A;
  const Point& B = t.AB.B;
  const Point& C = t.AC.B;

  return .5*std::abs(Cross(B-A,C-A));
}

Point Centroid(const Triangle& t)
{
  const Point& A = t.AB.A;
  const Point& B = t.AB.B;
  const Point& C = t.AC.B;

  return (A + B + C)/3.;;
}

Point ClosestPoint(const Point& p, const Triangle& t)
{
   if (Contains(t,p))
     return p;

   Point p1 = ClosestPoint(p,t.AB);
   double d1 = Distance(p1,p);
   Point p2 = ClosestPoint(p,t.BC);
   double d2 = Distance(p2,p);
   Point p3 = ClosestPoint(p,t.AC);
   double d3 = Distance(p3,p);

   return (d1 < d2 && d1 < d3) ? p1 : ((d2 < d3) ? p2 : p3);
}

double Distance(const Point& p, const Triangle& t)
{
  return Distance(p, ClosestPoint(p,t));
}

bool Coincident(const Triangle& t, const LineSegment& l)
{
  return (Coincident(t,l.A) || Coincident(t,l.B) ||
	  Coincident(t.AB,l) || Coincident(t.BC,l) || Coincident(t.AC,l));
}

bool Contains(const Triangle& t, const LineSegment& l)
{
  return Contains(t,l.A) && Contains(t,l.B);
}

bool Intersect(const LineSegment& l, const Triangle& t)
{
  return (Intersect(l,t.AB) ? true :
  	  Intersect(l,t.BC) ? true :
  	  Intersect(l,t.AC));
}

bool IntersectOrCoincident(const LineSegment& l, const Triangle& t)
{
  return (IntersectOrCoincident(l,t.AB) ? true :
  	  IntersectOrCoincident(l,t.BC) ? true :
  	  IntersectOrCoincident(l,t.AC));
}

std::tuple<unsigned, Point, Point> Intersection(const Triangle& t,
                                                const LineSegment& l)
{
  return Intersection(l,t);
}

std::tuple<unsigned, Point, Point> Intersection(const LineSegment& l,
                                                const Triangle& t)
{
  Point p[3] = {Intersection(l, t.AB),
                Intersection(l, t.BC),
                Intersection(l, t.AC)};

  if (!std::isnan(p[0].x))
  {
    if (!std::isnan(p[1].x))
    {
      if (!std::isnan(p[2].x))
      {
	// one of the points must be a vertex
	if (p[0] != p[1])
	  return std::make_tuple(2, p[0], p[1]);
	else // p[0] != p[2]
	  return std::make_tuple(2, p[0], p[2]);
      }
      else if (p[0] != p[1])
        return std::make_tuple(2, p[0], p[1]);
      else
      return std::make_tuple(1, p[0], p[2]);
    }
    else if (!std::isnan(p[2].x))
    {
      if (p[0] != p[2])
        return std::make_tuple(2, p[0], p[2]);
      else
        return std::make_tuple(1, p[0], p[1]);
    }
    else
      return std::make_tuple(1, p[0], p[1]);
  }
  else if (!std::isnan(p[1].x))
  {
    if (!std::isnan(p[2].x))
    {
      if (p[1] != p[2])
        return std::make_tuple(2, p[1], p[2]);
      else
        return std::make_tuple(1, p[1], p[0]);
    }
    else
      return std::make_tuple(1, p[1], p[0]);
  }
  else if (!std::isnan(p[2].x))
    return std::make_tuple(1, p[2], p[0]);
  else
    return std::make_tuple(0, p[0], p[1]);
}

Point ClosestPoint(const LineSegment& l, const Triangle& t)
{
  if (Contains(t,l.A))
    return l.A;
  else if (Contains(t,l.B))
    return l.B;
  else
  {
    Point p1 = ClosestPoint(l.A,t);
    double d1 = Distance(p1,l.A);
    Point p2 = ClosestPoint(l.B,t);
    double d2 = Distance(p2,l.B);
    return (d1 < d2 ? p1 : p2);
  }
}

double Distance(const LineSegment& l, const Triangle& t)
{
  return Distance(l, ClosestPoint(l,t));
}

bool Coincident(const Triangle& t1, const Triangle& t2)
{
  return Coincident(t2,t1.AB) || Coincident(t2,t1.BC) || Coincident(t2,t1.AC);
}

bool Contains(const Triangle& t1, const Triangle& t2)
{
  const Point& A = t1.AB.A;
  const Point& B = t1.AB.B;
  const Point& C = t1.AC.B;

  return Contains(t2,A) || Contains(t2,B) || Contains(t2,C);
}

bool Intersect(const Triangle& t1, const Triangle& t2)
{
  return (Intersect(t1.AB,t2) ? true :
	  Intersect(t1.BC,t2) ? true :
	  Intersect(t1.AC,t2));
}

Point ClosestPoint(const Triangle& t1, const Triangle& t2)
{
  const Point& A1 = t1.AB.A;
  const Point& B1 = t1.AB.B;
  const Point& C1 = t1.AC.B;

  if (Contains(t2,A1))
    return Point(A1);
  else if (Contains(t2,B1))
    return Point(B1);
  else if (Contains(t2,C1))
    return Point(C1);

  const Point& A2 = t2.AB.A;
  const Point& B2 = t2.AB.B;
  const Point& C2 = t2.AC.B;

  if (Contains(t1,A2))
    return Point(A2);
  else if (Contains(t1,B2))
    return Point(B2);
  else if (Contains(t1,C2))
    return Point(C2);

  Point p[3] = {ClosestPoint(t1.AB,t2),
		ClosestPoint(t1.BC,t2),
		ClosestPoint(t1.AC,t2)};
  double d[3] = {Distance(p[0],t2),
		 Distance(p[1],t2),
		 Distance(p[2],t2)};

  return (d[0] < d[1] && d[0] < d[2] ? p[0] : d[1] < d[2] ? p[1] : p[2]);
}

double Distance(const Triangle& t1, const Triangle& t2)
{
  const Point& A1 = t1.AB.A;
  const Point& B1 = t1.AB.B;
  const Point& C1 = t1.AC.B;

  if (Contains(t2,A1) || Contains(t2,B1) || Contains(t2,C1))
    return 0.;

  const Point& A2 = t2.AB.A;
  const Point& B2 = t2.AB.B;
  const Point& C2 = t2.AC.B;

  if (Contains(t1,A2) || Contains(t1,B2) || Contains(t1,C2))
    return 0.;

  double d1[3] = {Distance(t1.AB,t2),
		  Distance(t1.BC,t2),
		  Distance(t1.AC,t2)};

  unsigned c1 = (d1[0] < d1[1] && d1[0] < d1[2] ? 0 :
		 d1[1] < d1[2] ? 1 : 2);

  const LineSegment& l1 = (c1 == 0 ? t1.AB : c1 == 1 ? t1.BC : t1.AC);

  double d2[3] = {Distance(t2.AB,l1),
		  Distance(t2.BC,l1),
		  Distance(t2.AC,l1)};

  unsigned c2 = (d2[0] < d2[1] && d2[0] < d2[2] ? 0 :
		 d2[1] < d2[2] ? 1 : 2);

  const LineSegment& l2 = (c2 == 0 ? t2.AB : c2 == 1 ? t2.BC : t2.AC);

  return d1[c1] < d2[c2] ? d1[c1] : d2[c2];
}

}
}
