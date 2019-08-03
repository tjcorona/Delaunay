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

  Mesh::VertexList list;
  std::pair<const Mesh::Edge*, bool> firstEdge;
  for (Shape::PointList::const_iterator it = polygon.GetPoints().begin();
       it != polygon.GetPoints().end(); ++it)
  {
    Shape::PointList::const_iterator next = std::next(it);
    if (next == polygon.GetPoints().end())
      next = polygon.GetPoints().begin();
    Shape::LineSegment l(*it, *next);
    const Mesh::Edge* edge = insertLineSegment(l, mesh);
    list.push_back((static_cast<const Shape::Point&>(edge->A()) == *it ?
                    edge->A() : edge->B()));
    innerEdges.insert(edge);
    if (it == polygon.GetPoints().begin())
    {
      bool isCCW =
	Shape::Dot(edge->B() - edge->A(), *next - *it) > 0.;
      firstEdge = std::make_pair(edge, isCCW);
    }
  }

  RemoveBoundedRegion removeBoundedRegion;
  removeBoundedRegion(*firstEdge.first, firstEdge.second, mesh);

  this->GetInteriorBoundaries(mesh).emplace(list);

  return innerEdges;
}

}
}
