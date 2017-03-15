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

#ifndef DELAUNAY_SHAPE_CIRCLEUTILITIES_HH
#define DELAUNAY_SHAPE_CIRCLEUTILITIES_HH

#include <tuple>

#include "Shape/Export.hh"

namespace Delaunay
{
namespace Shape
{
class Circle;
class LineSegment;
class Point;
class Triangle;

// Does the point lie within the circle?
DELAUNAYSHAPE_EXPORT
bool Contains(const Circle&, const Point&);

// Does the circle intersect with the line segment?
DELAUNAYSHAPE_EXPORT
bool Intersect(const Circle&, const LineSegment&);

// Does the line segment intersect with the circle?
DELAUNAYSHAPE_EXPORT
bool Intersect(const LineSegment&, const Circle&);

// Does the circle intersect with the triangle?
DELAUNAYSHAPE_EXPORT
bool Intersect(const Circle&, const Triangle&);

// Does the triangle intersect with the circle?
DELAUNAYSHAPE_EXPORT
bool Intersect(const Triangle&, const Circle&);

// Do a line segment and a circle intersect? If so, how many times and where?
DELAUNAYSHAPE_EXPORT
std::tuple<unsigned, Point, Point> Intersection(const LineSegment&, const Circle&);
DELAUNAYSHAPE_EXPORT
std::tuple<unsigned, Point, Point> Intersection(const Circle&, const LineSegment&);
}
}

#endif
