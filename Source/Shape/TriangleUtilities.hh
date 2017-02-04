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

#ifndef DELAUNAY_SHAPE_TRIANGLEUTILITIES_HH
#define DELAUNAY_SHAPE_TRIANGLEUTILITIES_HH

#include "Shape/Point.hh"

namespace Delaunay
{
namespace Shape
{
class LineSegment;
class Triangle;

// Does the point lie on the perimeter of the triangle?
bool Coincident(const Triangle& t, const Point& p);

// Does the point lie within the triangle?
bool Contains(const Triangle& t, const Point& p);

// Return the area of the triangle.
double Area(const Triangle& t);

// Return the closest point on a triangle to a point.
Point ClosestPoint(const Point& p,const Triangle& t);
inline Point ClosestPoint(const Triangle& t,const Point& p)
{
  return ClosestPoint(p,t);
}

// Return the distance between a triangle and a point.
double Distance(const Point& p,const Triangle& t);
inline double Distance(const Triangle& t,const Point& p)
{
  return Distance(p,t);
}

// Does the line segment touch the triangle?
bool Coincident(const Triangle& t, const LineSegment& l);

// Does the line segment lie within the triangle?
bool Contains(const Triangle& t, const LineSegment& l);

// Does a line segment intersect a triangle?
bool Intersect(const LineSegment& l, const Triangle& t);
inline bool Intersect(const Triangle& t, const LineSegment& l)
{
  return Intersect(l,t);
}

// Do the two line segments intersect or touch?
bool IntersectOrCoincident(const LineSegment& l, const Triangle& t);
inline bool IntersectOrCoincident(const Triangle& t, const LineSegment& l)
{
  return IntersectOrCoincident(l,t);
}

// Do a line segment and a triangle intersect? If so, how many times and where?
std::tuple<unsigned, Point, Point> Intersection(const LineSegment&, const Triangle&);
std::tuple<unsigned, Point, Point> Intersection(const Triangle&, const LineSegment&);

// Return the closest point on a triangle to a line segment.
// NOPE: the parameter order should determine the shape on which the returned point lies
Point ClosestPoint(const LineSegment& l,const Triangle& t);
inline Point ClosestPoint(const Triangle& t,const LineSegment& l)
{
  return ClosestPoint(l,t);
}

// Return the distance between a triangle and a line segment.
double Distance(const LineSegment& l,const Triangle& t);
inline double Distance(const Triangle& t,const LineSegment& l)
{
  return Distance(l,t);
}

// Does the first triangle touch the second?
bool Coincident(const Triangle& t1,const Triangle& t2);

// Is the first triangle contained within the second?
bool Contains(const Triangle& t1,const Triangle& t2);

// Do the two triangles intersect?
bool Intersect(const Triangle& t1,const Triangle& t2);

// Return the closest point on a triangle to another triangle.
Point ClosestPoint(const Triangle& t1,const Triangle& t2);

// Return the distance between two triangles.
double Distance(const Triangle& t1,const Triangle& t2);
}
}

#endif
