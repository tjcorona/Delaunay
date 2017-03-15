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

#ifndef DELAUNAY_SHAPE_POINTUTILITIES_HH
#define DELAUNAY_SHAPE_POINTUTILITIES_HH

#include "Shape/Export.hh"

namespace Delaunay
{
namespace Shape
{
class Point;

// Returns the orientation of three points
// +1: counterclockwise
//  0: colinear
// -1: clockwise
DELAUNAYSHAPE_EXPORT
int Orientation(const Point& p1, const Point& p2, const Point& p3);

// Assuming both points are vector valued with initial point at the origin,
// return the dot product.
DELAUNAYSHAPE_EXPORT
double Dot(const Point& p1,const Point& p2);

// Assuming both points are vector valued with initial point at the origin,
// return the cross product Magnitude.
DELAUNAYSHAPE_EXPORT
double Cross(const Point& p1,const Point& p2);

// Return the square of the distance between two points.
DELAUNAYSHAPE_EXPORT
double DistanceSquared(const Point& p1, const Point& p2);

// Return the distance between two points.
DELAUNAYSHAPE_EXPORT
double Distance(const Point& p1, const Point& p2);

// Returns the angle in radians formed by the three points
DELAUNAYSHAPE_EXPORT
double Angle(const Point& p1, const Point& p2, const Point& p3);
}
}

#endif
