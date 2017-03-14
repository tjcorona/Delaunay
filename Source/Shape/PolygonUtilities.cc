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

#include "Shape/PolygonUtilities.hh"

#include <cassert>
#include <cmath>
#include <limits>

#include "Shape/PointUtilities.hh"
#include "Shape/Polygon.hh"

namespace Delaunay
{
namespace Shape
{
std::array<double, 4> Bounds(const Polygon& polygon)
{
  std::array<double, 4> bounds = {{std::numeric_limits<double>::max(),
                                   std::numeric_limits<double>::lowest(),
                                   std::numeric_limits<double>::max(),
                                   std::numeric_limits<double>::lowest()}};

  for (auto& point : polygon.GetPoints())
  {
    if (bounds[0] > point.get().x)
      bounds[0] = point.get().x;
    if (bounds[1] < point.get().x)
      bounds[1] = point.get().x;
    if (bounds[2] > point.get().y)
      bounds[2] = point.get().y;
    if (bounds[3] < point.get().y)
      bounds[3] = point.get().y;
  }

  return bounds;
}

int Orientation(const Polygon& polygon)
{
  // The first point in the polygon is guaranteed to be the point with the
  // smallest x-coordinate (and, if there are multiple points with this
  // x-coordinate, then it is the one with the smallest y-coordinate of this
  // subgroup). To determine the orientation of the polygon, we therefore only
  // need to cross the vectors that connect to this point.

  assert(polygon.GetPoints().size() >= 3);
  return Orientation(polygon.GetPoints().back(), polygon.GetPoints().front(),
                     polygon.GetPoints()[1]);
}

}
}
