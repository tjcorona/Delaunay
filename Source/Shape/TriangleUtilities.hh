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

#include "Shape/Export.hh"

#include "Shape/Point.hh"

namespace Delaunay
{
namespace Shape
{
class LineSegment;
class Triangle;

// Does the point lie on the perimeter of the triangle?
DELAUNAYSHAPE_EXPORT
bool Coincident(const Triangle& t, const Point& p);

// Does the point lie within the triangle?
DELAUNAYSHAPE_EXPORT
bool Contains(const Triangle& t, const Point& p);

// Return the area of the triangle.
DELAUNAYSHAPE_EXPORT
double Area(const Triangle& t);

// Return the centroid of the triangle;
DELAUNAYSHAPE_EXPORT
Point Centroid(const Triangle& t);

// Return the closest point on a triangle to a point.
DELAUNAYSHAPE_EXPORT
Point ClosestPoint(const Point& p,const Triangle& t);
DELAUNAYSHAPE_EXPORT
inline Point ClosestPoint(const Triangle& t,const Point& p)
{
  return ClosestPoint(p,t);
}

// Return the distance between a triangle and a point.
DELAUNAYSHAPE_EXPORT
double Distance(const Point& p,const Triangle& t);
DELAUNAYSHAPE_EXPORT
inline double Distance(const Triangle& t,const Point& p)
{
  return Distance(p,t);
}

// Does the line segment touch the triangle?
DELAUNAYSHAPE_EXPORT
bool Coincident(const Triangle& t, const LineSegment& l);

// Does the line segment lie within the triangle?
DELAUNAYSHAPE_EXPORT
bool Contains(const Triangle& t, const LineSegment& l);

// Does a line segment intersect a triangle?
DELAUNAYSHAPE_EXPORT
bool Intersect(const LineSegment& l, const Triangle& t);
DELAUNAYSHAPE_EXPORT
inline bool Intersect(const Triangle& t, const LineSegment& l)
{
  return Intersect(l,t);
}

// Do the two line segments intersect or touch?
DELAUNAYSHAPE_EXPORT
bool IntersectOrCoincident(const LineSegment& l, const Triangle& t);
DELAUNAYSHAPE_EXPORT
inline bool IntersectOrCoincident(const Triangle& t, const LineSegment& l)
{
  return IntersectOrCoincident(l,t);
}

// Do a line segment and a triangle intersect? If so, how many times and where?
DELAUNAYSHAPE_EXPORT
std::tuple<unsigned, Point, Point> Intersection(const LineSegment&, const Triangle&);
DELAUNAYSHAPE_EXPORT
std::tuple<unsigned, Point, Point> Intersection(const Triangle&, const LineSegment&);

// Return the closest point on a triangle to a line segment.
// NOPE: the parameter order should determine the shape on which the returned point lies
DELAUNAYSHAPE_EXPORT
Point ClosestPoint(const LineSegment& l,const Triangle& t);
DELAUNAYSHAPE_EXPORT
inline Point ClosestPoint(const Triangle& t,const LineSegment& l)
{
  return ClosestPoint(l,t);
}

// Return the distance between a triangle and a line segment.
DELAUNAYSHAPE_EXPORT
double Distance(const LineSegment& l,const Triangle& t);
DELAUNAYSHAPE_EXPORT
inline double Distance(const Triangle& t,const LineSegment& l)
{
  return Distance(l,t);
}

// Does the first triangle touch the second?
DELAUNAYSHAPE_EXPORT
bool Coincident(const Triangle& t1,const Triangle& t2);

// Is the first triangle contained within the second?
DELAUNAYSHAPE_EXPORT
bool Contains(const Triangle& t1,const Triangle& t2);

// Do the two triangles intersect?
DELAUNAYSHAPE_EXPORT
bool Intersect(const Triangle& t1,const Triangle& t2);

// Return the closest point on a triangle to another triangle.
DELAUNAYSHAPE_EXPORT
Point ClosestPoint(const Triangle& t1,const Triangle& t2);

// Return the distance between two triangles.
DELAUNAYSHAPE_EXPORT
double Distance(const Triangle& t1,const Triangle& t2);
}
}

#endif
