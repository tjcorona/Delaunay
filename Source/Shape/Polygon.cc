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

#include "Polygon.hh"

#include "Shape/LineSegment.hh"

#include <algorithm>

namespace
{
typedef Delaunay::Shape::PointVector PointVector;

template <class Container>
PointVector Sort(const Container& points)
{
  PointVector sortedPoints;

  unsigned positionOfSmallest =
    std::distance(points.begin(), std::min_element(points.begin(),
						   points.end()));
  unsigned size = points.size();
  for (unsigned i=0;i<size;i++)
  {
    sortedPoints.push_back(std::cref(points[(i+positionOfSmallest)%size]));
  }

  return sortedPoints;
}
}

namespace Delaunay
{
namespace Shape
{

Polygon::Polygon(const std::vector<Point>& points) :
  Points(Sort(points))
{
}

Polygon::Polygon(const PointVector& points) :
  Points(Sort(points))
{
}

bool Polygon::Contains(const Point& p) const
{
  (void)p;
  return false;
}

double Polygon::Distance(const Point& p) const
{
  (void)p;
  return 0.;
}

}
}
