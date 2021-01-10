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

#include "Validation/IsValidPolygon.hh"

#include "Shape/LineSegment.hh"
#include "Shape/LineSegmentUtilities.hh"

namespace Delaunay
{
namespace Validation
{

bool IsValidPolygon::operator()(const Delaunay::Shape::Polygon& polygon) const
{
  for (Shape::PointList::const_iterator it = polygon.GetPoints().begin();
       it != polygon.GetPoints().end(); ++it)
  {
    Shape::PointList::const_iterator next = std::next(it);
    if (next == polygon.GetPoints().end())
      next = polygon.GetPoints().begin();

    for (Shape::PointList::const_iterator it2 = next;
         it2 != polygon.GetPoints().end(); ++it2)
    {
      // Check for duplicate points
      if (*it == *it2)
        return false;

      Shape::PointList::const_iterator next2 = std::next(it2);
      if (next2 == polygon.GetPoints().end())
        next2 = polygon.GetPoints().begin();

      // Check for intersecting edges
      if (Shape::Intersect(Shape::LineSegment(*it,*next),
                           Shape::LineSegment(*it2, *next2)))
        return false;
    }
  }

  return true;
}

}
}
