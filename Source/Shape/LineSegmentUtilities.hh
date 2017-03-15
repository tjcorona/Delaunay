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

#ifndef DELAUNAY_SHAPE_LINESEGMENTUTILITIES_HH
#define DELAUNAY_SHAPE_LINESEGMENTUTILITIES_HH

#include "Shape/Export.hh"

#include "Shape/Point.hh"

namespace Delaunay
{
namespace Shape
{
class LineSegment;

// Does the point lie on the line segment?
DELAUNAYSHAPE_EXPORT
bool Contains(const LineSegment& l, const Point& p);

// Return the length squared of the line segment.
DELAUNAYSHAPE_EXPORT
double LengthSquared(const LineSegment& l);

// Return the length of the line segment.
DELAUNAYSHAPE_EXPORT
double Length(const LineSegment& l);

// Return the closest point on a line segment to a point.
DELAUNAYSHAPE_EXPORT
Point ClosestPoint(const Point& p,const LineSegment& l);
DELAUNAYSHAPE_EXPORT
inline Point ClosestPoint(const LineSegment& l,const Point& p)
{
  return ClosestPoint(p,l);
}

// Return the distance between a line segment and a point.
DELAUNAYSHAPE_EXPORT
double Distance(const Point& p,const LineSegment& l);
DELAUNAYSHAPE_EXPORT
inline double Distance(const LineSegment& l,const Point& p)
{
  return Distance(p,l);
}

// Do the two line segments touch?
DELAUNAYSHAPE_EXPORT
bool Coincident(const LineSegment& l1,const LineSegment& l2);

// Do the two line segments intersect?
DELAUNAYSHAPE_EXPORT
bool Intersect(const LineSegment& l1,const LineSegment& l2);

// Do the two line segments intersect or touch?
DELAUNAYSHAPE_EXPORT
bool IntersectOrCoincident(const LineSegment& l1,const LineSegment& l2);

// Return the point of intersection for two line segments.
DELAUNAYSHAPE_EXPORT
Point Intersection(const LineSegment& l1,const LineSegment& l2);

// Return a closest point on the first line segment to the second line
// segment.
DELAUNAYSHAPE_EXPORT
Point ClosestPoint(const LineSegment& l1,const LineSegment& l2);

// Return the distance between two line segments.
DELAUNAYSHAPE_EXPORT
double Distance(const LineSegment& l1,const LineSegment& l2);
}
}

#endif
