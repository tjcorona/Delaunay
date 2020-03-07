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

#include "ConstrainedDelaunayMesh.hh"

#include "Discretization/InsertLineSegment.hh"
#include "Discretization/RemoveBoundedRegion.hh"
#include "Shape/PolygonUtilities.hh"

#include <limits>

namespace Delaunay
{
namespace Discretization
{
void ConstrainedDelaunayMesh::operator()(
  const Delaunay::Shape::Polygon& polygon, Delaunay::Mesh::Mesh& mesh)
{
  std::array<double, 4> bounds(Shape::Bounds(polygon));
  double xLen = bounds[1] - bounds[0];
  double yLen = bounds[3] - bounds[2];
  bounds[0] -= .2*xLen;
  bounds[1] += .2*xLen;
  bounds[2] -= .2*yLen;
  bounds[3] += .2*yLen;

  bool inSitu = mesh.GetVertices().empty();

  Mesh::Mesh mesh_;
  Mesh::Mesh* augmentedMesh;
  if (inSitu)
    augmentedMesh = &mesh;
  else
    augmentedMesh = &mesh_;

  const Mesh::Vertex& v0 = *(this->InsertVertex(
                               Shape::Point(bounds[0],bounds[2]),
                               *augmentedMesh)).first;
  const Mesh::Vertex& v1 = *(this->InsertVertex(
                               Shape::Point(bounds[1],bounds[2]),
                               *augmentedMesh)).first;
  const Mesh::Vertex& v2 = *(this->InsertVertex(
                               Shape::Point(bounds[0],bounds[3]),
                               *augmentedMesh)).first;
  const Mesh::Vertex& v3 = *(this->InsertVertex(
                               Shape::Point(bounds[1],bounds[3]),
                               *augmentedMesh)).first;

  const Mesh::Edge& e01 =*(this->InsertEdge(v0,v1,*augmentedMesh)).first;
  const Mesh::Edge& e02 =*(this->InsertEdge(v0,v2,*augmentedMesh)).first;
  const Mesh::Edge& e12 =*(this->InsertEdge(v1,v2,*augmentedMesh)).first;
  const Mesh::Edge& e13 =*(this->InsertEdge(v1,v3,*augmentedMesh)).first;
  const Mesh::Edge& e23 =*(this->InsertEdge(v2,v3,*augmentedMesh)).first;

  this->GetTriangles(*augmentedMesh).emplace(e01,e02,e12);
  this->GetTriangles(*augmentedMesh).emplace(e13,e23,e12);

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
    list.push_back(std::cref(edge->A() == *it ? edge->A() : edge->B()));
    if (it == polygon.GetPoints().begin())
    {
      bool isCCW =
	Shape::Dot(edge->B() - edge->A(), *next - *it) > 0.;
      firstEdge = std::make_pair(edge, isCCW);
    }
  }

  RemoveBoundedRegion removeBoundedRegion;
  removeBoundedRegion(*firstEdge.first, !firstEdge.second, *augmentedMesh);

  if (inSitu)
  {
    this->GetPerimeter(mesh).SetVertices(list);
  }
  else
  {
    RemoveBoundedRegion removeBoundedRegion;
    removeBoundedRegion(*firstEdge.first, !firstEdge.second, *augmentedMesh);

    if (inSitu)
    {
      this->GetPerimeter(mesh).SetVertices(list);
    }
    else
    {
      for (auto& triangle : mesh_.GetTriangles())
      {
        const Mesh::Vertex& a = *(this->InsertVertex(
                                    triangle.AB().A(),mesh)).first;
        const Mesh::Vertex& b = *(this->InsertVertex(
                                    triangle.AB().B(),mesh)).first;
        const Mesh::Vertex& c = *(this->InsertVertex(
                                    triangle.AC().B(),mesh)).first;
        const Mesh::Edge& ab = *(this->InsertEdge(a,b,mesh)).first;
        const Mesh::Edge& bc = *(this->InsertEdge(b,c,mesh)).first;
        const Mesh::Edge& ac = *(this->InsertEdge(a,c,mesh)).first;
        this->InsertTriangle(ab,bc,ac,mesh);
      }
    }
  }
}

}
}
