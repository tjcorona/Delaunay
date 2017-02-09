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

#include "Discretization/ExtendMesh.hh"

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

void ExtendMesh::operator()(const Point& p, Delaunay::Mesh::Mesh& mesh)
{
  Shape::PointVector vec(this->GetPerimeter(mesh).GetPoints());

  const Mesh::Vertex& vtx = *(this->GetVertices(mesh).emplace(p)).first;

  this->GetPerimeter(mesh).SetPoints(vec);
  this->GetVertices(mesh).insert(vtx);

  if (this->GetVertices(mesh).size()<3)
return;

  if (this->GetVertices(mesh).size()==3)
  {
    const Mesh::Vertex* vs[3];
    Mesh::Mesh::VertexSet::iterator it = this->GetVertices(mesh).begin();
    for (unsigned i=0;i<3;i++)
      vs[i] = &(*it++);

    const Mesh::Edge& ab =*(this->GetEdges(mesh).emplace(*vs[0],*vs[1])).first;
    const Mesh::Edge& bc =*(this->GetEdges(mesh).emplace(*vs[1],*vs[2])).first;
    const Mesh::Edge& ac =*(this->GetEdges(mesh).emplace(*vs[0],*vs[2])).first;
    this->GetTriangles(mesh).emplace(ab,bc,ac);

    return;
  }
  Extend(&vtx, mesh);
}

void ExtendMesh::Extend(const Mesh::Vertex* v, Delaunay::Mesh::Mesh& mesh)
{
  // append vertex v, which is exterior to the extant mesh, to the mesh

  double minPerimeterToArea = std::numeric_limits<double>::max();
  const Mesh::Edge* minEdge = nullptr;

  const Mesh::Vertex* vlast = nullptr;
  const Mesh::Vertex* vi = &(*(this->GetVertices(mesh).begin()));

  if (vi == v)
    vi = &(*(++this->GetVertices(mesh).begin()));

  unsigned vtx_counter = 0;

  const Mesh::Vertex* vfirst = vi;

  do
  {
    Mesh::EdgeSet::iterator edge = vi->edges.begin();
    while ((*edge)->triangles.size() != 1 ||
           vlast == &((*edge)->A()) || vlast == &((*edge)->B()))
    {
      edge++;
    }

    bool legal = false;
    {
      // check if a new triangle with this edge and vertex is legal
      const Mesh::Vertex* va = &((*edge)->A());
      const Mesh::Vertex* vb = &((*edge)->B());
      const Mesh::Vertex* vopp;
      const Mesh::Triangle* t = *((*edge)->triangles.begin());
      vopp = &(t->A());
      if (vopp == &((*edge)->A()) || vopp == &((*edge)->B()))
        vopp = &(t->B());
      if (vopp == &((*edge)->A()) || vopp == &((*edge)->B()))
        vopp = &(t->C());

      bool b1 = (v->x-vb->x)*(va->y-vb->y) - (va->x-vb->x)*(v->y-vb->y) < 0.;
      bool b2 = (vopp->x-vb->x)*(va->y-vb->y) - (va->x-vb->x)*(vopp->y-vb->y) < 0.;
      legal = (b1 != b2);
    }

    if (legal)
    {
      const Mesh::Vertex& A = (*edge)->A();
      const Mesh::Vertex& B = (*edge)->B();
      const Mesh::Vertex& C = *v;
      double perimeter = Distance(A,B) + Distance(A,C) + Distance(B,C);
      double area = fabs(A.x*(B.y-C.y) + B.x*(C.y-A.y) + C.x*(A.y-B.y))*.5;
      double perimeterToArea = perimeter/area;

      if (perimeterToArea < minPerimeterToArea)
        minEdge = (*edge);
    }

    vlast = vi;
    vi = (vi == &((*edge)->A()) ? &((*edge)->B()) : &((*edge)->A()));
  }
  while (vi != vfirst);

  // Edges, triangles should be returned via a convenience method in Mesher
  const Mesh::Edge& va = *(this->GetEdges(mesh)
                           .emplace(*v, minEdge->A())).first;
  const Mesh::Edge& vb = *(this->GetEdges(mesh)
                           .emplace(*v, minEdge->B())).first;
  const Mesh::Triangle& t = *(this->GetTriangles(mesh).emplace(va,vb,*minEdge)).first;
  this->GetVertices(mesh).insert(*v);

  {
    const Mesh::Vertex& A = t.A();
    const Mesh::Vertex& B = t.B();
    const Mesh::Vertex& C = t.C();

    std::set<const Mesh::Edge*> edges;
    for (Mesh::EdgeSet::iterator edge=A.edges.begin();edge!=A.edges.end();++edge)
      edges.insert(*edge);
    for (Mesh::EdgeSet::iterator edge=B.edges.begin();edge!=B.edges.end();++edge)
      edges.insert(*edge);
    for (Mesh::EdgeSet::iterator edge=C.edges.begin();edge!=C.edges.end();++edge)
      edges.insert(*edge);

    LegalizeEdges legalizeEdges;
    legalizeEdges(v,edges,mesh);
  }

  return;
}

}
}
