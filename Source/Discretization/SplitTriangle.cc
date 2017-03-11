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

#include "Discretization/SplitTriangle.hh"
#include "Discretization/LegalizeEdges.hh"

#include <iostream>

namespace Delaunay
{
namespace Discretization
{

const Mesh::Vertex* SplitTriangle::operator()(const Mesh::Triangle& t,
                                              Delaunay::Mesh::Mesh& mesh) const
{
  Shape::Point p = (t.AB().A() + t.AB().B() + t.AC().B())/3.;
  return this->operator()(t,p,mesh);
}

const Mesh::Vertex* SplitTriangle::operator()(const Mesh::Triangle& t,
                                              double barycentricCoords[3],
                                              Delaunay::Mesh::Mesh& mesh) const
{
  Shape::Point p = (barycentricCoords[0]*t.AB().A() +
                    barycentricCoords[1]*t.AB().B() +
                    barycentricCoords[2]*t.AC().B());
  return this->operator()(t,p,mesh);
}

const Mesh::Vertex* SplitTriangle::operator()(const Mesh::Triangle& t,
                                              const Shape::Point& p,
                                              Delaunay::Mesh::Mesh& mesh) const
{
  // split Triangle t into 3 new triangles using vertex v, remove t from the
  // triangle set and add the 3 new triangles to the triangle set
  const Mesh::Vertex* v = nullptr;

  auto result = this->GetVertices(mesh).emplace(p);
  v = &(*result.first);

  if (!result.second)
    return v;

  const Mesh::Vertex& A = t.A();
  const Mesh::Vertex& B = t.B();
  const Mesh::Vertex& C = t.C();
  const Mesh::Vertex& D = *v;

  const Mesh::Edge& AB = t.AB();
  const Mesh::Edge& BC = t.BC();
  const Mesh::Edge& AC = t.AC();

  const Mesh::Edge& AD =*(this->GetEdges(mesh).emplace(A,D)).first;
  const Mesh::Edge& BD =*(this->GetEdges(mesh).emplace(B,D)).first;
  const Mesh::Edge& CD =*(this->GetEdges(mesh).emplace(C,D)).first;

  this->GetTriangles(mesh).emplace(AB,BD,AD);
  this->GetTriangles(mesh).emplace(AC,CD,AD);
  this->GetTriangles(mesh).emplace(BC,CD,BD);

  this->GetTriangles(mesh).erase(t);

  std::set<const Mesh::Edge*> edges;
  if (!AB.boundary)
    edges.insert(&AB);
  if (!BC.boundary)
    edges.insert(&BC);
  if (!AC.boundary)
    edges.insert(&AC);

  static const LegalizeEdges legalizeEdges;
  legalizeEdges(v, edges, mesh);

  return v;
}

}
}
