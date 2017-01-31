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

#include "Shape/Point.hh"

namespace Delaunay
{
namespace Shape
{
class LineSegment;

// Does the point lie on the line segment?
bool Contains(const LineSegment& l, const Point& p);

// Return the length squared of the line segment.
double LengthSquared(const LineSegment& l);

// Return the length of the line segment.
double Length(const LineSegment& l);

// Return the closest point on a line segment to a point.
Point ClosestPoint(const Point& p,const LineSegment& l);
inline Point ClosestPoint(const LineSegment& l,const Point& p)
{
  return ClosestPoint(p,l);
}

// Return the distance between a line segment and a point.
double Distance(const Point& p,const LineSegment& l);
inline double Distance(const LineSegment& l,const Point& p)
{
  return Distance(p,l);
}

// Do the two line segments touch?
bool Coincident(const LineSegment& l1,const LineSegment& l2);

// Do the two line segments intersect?
bool Intersect(const LineSegment& l1,const LineSegment& l2);

// Do the two line segments intersect or touch?
bool IntersectOrCoincident(const LineSegment& l1,const LineSegment& l2);

// Return a closest point on the first line segment to the second line
// segment.
Point ClosestPoint(const LineSegment& l1,const LineSegment& l2);

// Return the distance between two line segments.
double Distance(const LineSegment& l1,const LineSegment& l2);
}
}

#endif
