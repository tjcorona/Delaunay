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

#include "Discretization/AddInteriorPoint.hh"

#include "Shape/CircleUtilities.hh"
#include "Shape/PointUtilities.hh"
#include "Shape/TriangleUtilities.hh"

#include "Discretization/LegalizeEdges.hh"

#include <cassert>
#include <exception>
#include <limits>

namespace Delaunay
{
namespace Discretization
{

const Mesh::Vertex* AddInteriorPoint::operator()(const Point& p,
						 Delaunay::Mesh::Mesh& mesh)
{
  assert(this->GetTriangles(mesh).size() != 0);

  const Mesh::Triangle* containingTriangle = FindContainingTriangle(p,mesh);

  const Mesh::Vertex* vtx = nullptr;
  if (containingTriangle)
  {
    auto result = this->GetVertices(mesh).emplace(p);
    vtx = &(*result.first);
    if (result.second)
    {
      SplitTriangle(containingTriangle, vtx, mesh);
    }
  }
  return vtx;
}

const Mesh::Triangle* AddInteriorPoint::FindContainingTriangle(
  const Point& p, Delaunay::Mesh::Mesh& mesh) const
{
  for (auto it = mesh.GetTriangles().begin();
       it != mesh.GetTriangles().end(); ++it)
    if (Contains(*it, p))
      return &(*it);

  return nullptr;
}

void AddInteriorPoint::SplitTriangle(const Mesh::Triangle* t,
				     const Mesh::Vertex* v,
				     Delaunay::Mesh::Mesh& mesh)
{
  // split Triangle t into 3 new triangles using vertex v, remove t from the
  // triangle set and add the 3 new triangles to the triangle set
  const Mesh::Vertex& A = t->A();
  const Mesh::Vertex& B = t->B();
  const Mesh::Vertex& C = t->C();
  const Mesh::Vertex& D = *v;

  const Mesh::Edge& AB = t->AB();
  const Mesh::Edge& BC = t->BC();
  const Mesh::Edge& AC = t->AC();

  const Mesh::Edge& AD =*(this->GetEdges(mesh).emplace(A,D)).first;
  const Mesh::Edge& BD =*(this->GetEdges(mesh).emplace(B,D)).first;
  const Mesh::Edge& CD =*(this->GetEdges(mesh).emplace(C,D)).first;

  this->GetTriangles(mesh).emplace(AB,BD,AD);
  this->GetTriangles(mesh).emplace(AC,CD,AD);
  this->GetTriangles(mesh).emplace(BC,CD,BD);

  this->GetTriangles(mesh).erase(*t);

  std::set<const Mesh::Edge*> edges;
  if (!AB.boundary)
    edges.insert(&AB);
  if (!BC.boundary)
    edges.insert(&BC);
  if (!AC.boundary)
    edges.insert(&AC);

  LegalizeEdges legalizeEdges;
  legalizeEdges(v, edges, mesh);
}

}
}
