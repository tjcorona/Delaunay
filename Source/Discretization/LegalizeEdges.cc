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

#include "Discretization/LegalizeEdges.hh"

#include "Shape/CircleUtilities.hh"
#include "Shape/PointUtilities.hh"
#include "Shape/TriangleUtilities.hh"

#include <cassert>
#include <exception>
#include <limits>

namespace Delaunay
{
namespace Discretization
{

void LegalizeEdges::operator()(const Mesh::Vertex* v,
			       std::set<const Mesh::Edge*>& edges,
			       Delaunay::Mesh::Mesh& mesh) const
{
  if (edges.empty())
    return;

  const Mesh::Edge* edge = *(edges.begin());
  edges.erase(edges.begin());

  assert(edge->triangles.size()!=0);
  assert(edge->triangles.size()<=2);

  // Now that boundaries are excluded from legalization, this check is probably
  // not needed.
  if (edge->triangles.size() == 1 || edge->boundary)
  {
    return this->operator()(v,edges,mesh);
  }

  const Mesh::Triangle* t1 = *(edge->triangles.begin());
  const Mesh::Triangle* t2 = *(++(edge->triangles.begin()));

  const Mesh::Vertex* I = &(edge->A());
  const Mesh::Vertex* J = &(edge->B());
  const Mesh::Vertex* K = &(t1->A());
  if (K == I || K == J)
    K = &(t1->B());
  if (K == I || K == J)
    K = &(t1->C());
  const Mesh::Vertex* L = &(t2->A());
  if (L == I || L == J)
    L = &(t2->B());
  if (L == I || L == J)
    L = &(t2->C());

  const Mesh::Edge* ik = nullptr;
  const Mesh::Edge* jk = nullptr;
  const Mesh::Edge* il = nullptr;
  const Mesh::Edge* jl = nullptr;

  bool isLegal = true;

  const Shape::Point* C = &(t1->circumcircle.Center);

  double d2 = (C->x-L->x)*(C->x-L->x) + (C->y-L->y)*(C->y-L->y);
  if (v == L)
  {
    if (d2 + EPSILON < t1->circumcircle.Radius*t1->circumcircle.Radius)
    {
      isLegal = false;
      for (Mesh::EdgeSet::iterator e=I->edges.begin();e!=I->edges.end();++e)
        if (&((*e)->A()) == K || &((*e)->B()) == K)
        {
          ik = *e;
          edges.insert(*e);
        }
      for (Mesh::EdgeSet::iterator e=J->edges.begin();e!=J->edges.end();++e)
        if (&((*e)->A()) == K || &((*e)->B()) == K)
        {
          jk = *e;
          edges.insert(*e);
        }
      il = &(*(this->GetEdges(mesh).emplace(*I, *L)).first);
      jl = &(*(this->GetEdges(mesh).emplace(*J, *L)).first);
    }
  }

  if (v == K)
  {
    C = &(t2->circumcircle.Center);
    d2 = (C->x-K->x)*(C->x-K->x) + (C->y-K->y)*(C->y-K->y);
    if (d2 + EPSILON < t2->circumcircle.Radius*t2->circumcircle.Radius)
    {
      isLegal = false;

      for (Mesh::EdgeSet::iterator e=I->edges.begin();e!=I->edges.end();++e)
        if (&((*e)->A()) == L || &((*e)->B()) == L)
        {
          il = *e;
          edges.insert(*e);
        }
      for (Mesh::EdgeSet::iterator e=J->edges.begin();e!=J->edges.end();++e)
        if (&((*e)->A()) == L || &((*e)->B()) == L)
        {
          jl = *e;
          edges.insert(*e);
        }
      ik = &(*(this->GetEdges(mesh).emplace(*I, *K)).first);
      jk = &(*(this->GetEdges(mesh).emplace(*J, *K)).first);
    }
  }

  if (!isLegal)
  {
    const Mesh::Edge* kl = &(*(this->GetEdges(mesh).emplace(*K, *L)).first);

    this->GetEdges(mesh).emplace(*kl);
    this->GetTriangles(mesh).erase(*t1);
    this->GetTriangles(mesh).erase(*t2);
    this->GetEdges(mesh).erase(*edge);
    this->GetTriangles(mesh).emplace(*ik, *kl, *il);
    this->GetTriangles(mesh).emplace(*jk, *kl, *jl);
  }
  return this->operator()(v,edges,mesh);
}

}
}
