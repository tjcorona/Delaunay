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

bool IsValidPolygon::operator()(const Delaunay::Shape::Polygon& poly) const
{
  std::size_t nPoints = poly.GetPoints().size();

  for (std::size_t i=0; i<nPoints; i++)
  {
    std::size_t ipp = (i+1)%nPoints;
    for (std::size_t j=i+1; j<nPoints; j++)
    {
      // Check for duplicate points
      if (poly.GetPoints()[i] == poly.GetPoints()[j])
        return false;

      std::size_t jpp = (j+1)%nPoints;

      // Check for intersecting edges
      if (Shape::Intersect(
            Shape::LineSegment(poly.GetPoints()[i],poly.GetPoints()[ipp]),
            Shape::LineSegment(poly.GetPoints()[j], poly.GetPoints()[jpp])))
        return false;
    }
  }

  return true;
}

}
}
