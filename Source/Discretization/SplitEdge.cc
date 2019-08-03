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

#include "Discretization/SplitEdge.hh"
#include "Discretization/LegalizeEdges.hh"

#include <array>
#include <cassert>

namespace Delaunay
{
namespace Discretization
{

std::pair<const Mesh::Edge*, const Mesh::Edge*>
SplitEdge::operator()(const Mesh::Edge& edge, Delaunay::Mesh::Mesh& mesh) const
{
  return this->operator()(edge, .5, mesh);
}

namespace
{
const Mesh::Edge* GetEdge(const Mesh::Vertex* v1, const Mesh::Vertex* v2,
			  const Mesh::Triangle* t)
{
  std::array<const Mesh::Edge*, 3> edges =
    {{&(t->AB()), &(t->BC()), &(t->AC())}};

  for (auto& edge : edges)
    if ((&edge->A() == v1 && &edge->B() == v2) ||
	(&edge->B() == v1 && &edge->A() == v2))
      return edge;

  return nullptr;
}
}

std::pair<const Mesh::Edge*, const Mesh::Edge*>
SplitEdge::operator()(const Mesh::Edge& edge, double t,
		      Delaunay::Mesh::Mesh& mesh) const
{
  assert(t >= -EPSILON/2. && t <= 1. + EPSILON/2.);

  if (t < EPSILON || std::abs(t - 1.) < EPSILON)
    return std::make_pair<const Mesh::Edge*,const Mesh::Edge*>(nullptr,nullptr);

  const Mesh::Vertex& v =
		 *(this->GetVertices(mesh).emplace((edge.A()*(1. - t) +
						    edge.B()*t)).first);
  const Mesh::Edge& esA = *(this->GetEdges(mesh).emplace(v,edge.A()).first);
  const Mesh::Edge& esB = *(this->GetEdges(mesh).emplace(v,edge.B()).first);

  if (edge.boundary)
  {
    const_cast<Mesh::Edge&>(esA).boundary = true;
    const_cast<Mesh::Edge&>(esB).boundary = true;

    if (this->GetPerimeter(mesh).GetVertices().find(edge.A()) !=
        this->GetPerimeter(mesh).GetVertices().end())
    {
      this->GetPerimeter(mesh).GetVertices().insert(edge.A(), edge.B(), v);
    }
    else
    {
      // set iteration must be const
      for (const Mesh::Polygon& boundary : this->GetInteriorBoundaries(mesh))
      {
        if (boundary.GetVertices().find(edge.A()) !=
            boundary.GetVertices().end())
        {
          const_cast<Mesh::Polygon&>(boundary).GetVertices()
            .insert(edge.A(), edge.B(), v);
          break;
        }
      }
    }
  }

  std::set<const Mesh::Edge*> toLegalize;

  Mesh::TriangleSet triangles = edge.triangles;
  for (auto& triangle : triangles)
  {
    const Mesh::Vertex* common = &(triangle->AB().A());
    if (common == &edge.A() || common == &edge.B())
      common = &(triangle->AB().B());
    if (common == &edge.A() || common == &edge.B())
      common = &(triangle->AC().B());

    const Mesh::Edge* eA = GetEdge(&edge.A(), common, triangle);
    const Mesh::Edge* eB = GetEdge(&edge.B(), common, triangle);

    assert(eA);
    assert(eB);

    toLegalize.insert(eA);
    toLegalize.insert(eB);

    const Mesh::Edge& ec =  *(this->GetEdges(mesh).emplace(*common, v)).first;

    this->GetTriangles(mesh).emplace(esA,*eA,ec);
    this->GetTriangles(mesh).emplace(esB,*eB,ec);

    this->GetTriangles(mesh).erase(*triangle);
  }

  this->GetEdges(mesh).erase(edge);

  LegalizeEdges legalizeEdges;
  legalizeEdges(&v, toLegalize, mesh);

  return std::make_pair(&esA,&esB);
}

}
}
