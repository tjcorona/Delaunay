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

#include "Discretization/ExcisePolygon.hh"

#include "Discretization/InsertLineSegment.hh"
#include "Discretization/RemoveBoundedRegion.hh"
#include "Mesh/Edge.hh"

#include <cassert>

namespace Delaunay
{
namespace Discretization
{

std::set<const Mesh::Edge*> ExcisePolygon::operator()(
  const Delaunay::Shape::Polygon& polygon, Delaunay::Mesh::Mesh& mesh)
{
  std::set<const Mesh::Edge*> innerEdges;
  InsertLineSegment insertLineSegment;

  std::pair<const Mesh::Edge*, bool> firstEdge = std::make_pair(nullptr, false);
  for (unsigned i = 0; i < polygon.GetPoints().size(); ++i)
  {
    unsigned ipp = (i+1)%polygon.GetPoints().size();
    Shape::LineSegment l(polygon.GetPoints()[i], polygon.GetPoints()[ipp]);
    const Mesh::Edge* edge = insertLineSegment(l, mesh);
    innerEdges.insert(edge);
    if (i == 0)
    {
      bool isCCW =
	Shape::Dot(edge->B() - edge->A(),
		   polygon.GetPoints()[ipp] - polygon.GetPoints()[i]) > 0.;
      firstEdge = std::make_pair(edge, isCCW);
    }
  }

  if (firstEdge.first != nullptr)
  {
    RemoveBoundedRegion removeBoundedRegion;
    removeBoundedRegion(*firstEdge.first, firstEdge.second, mesh);
  }

  return innerEdges;
}

}
}
