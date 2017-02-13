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

#include "Discretization/InsertEdge.hh"
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
  std::array<double, 4> bounds(std::move(Shape::Bounds(polygon)));
  double xLen = bounds[1] - bounds[0];
  double yLen = bounds[3] - bounds[2];
  bounds[0] -= .1*xLen;
  bounds[1] += .1*xLen;
  bounds[2] -= .1*yLen;
  bounds[3] += .1*yLen;

  bool inSitu = mesh.GetVertices().empty();

  Mesh::Mesh mesh_;
  Mesh::Mesh* augmentedMesh;
  if (inSitu)
    augmentedMesh = &mesh;
  else
    augmentedMesh = &mesh_;

  const Mesh::Vertex& v0 = *(this->GetVertices(*augmentedMesh).emplace(
			       Shape::Point(bounds[0],bounds[2]))).first;
  const Mesh::Vertex& v1 = *(this->GetVertices(*augmentedMesh).emplace(
			       Shape::Point(bounds[1],bounds[2]))).first;
  const Mesh::Vertex& v2 = *(this->GetVertices(*augmentedMesh).emplace(
			       Shape::Point(bounds[0],bounds[3]))).first;
  const Mesh::Vertex& v3 = *(this->GetVertices(*augmentedMesh).emplace(
			       Shape::Point(bounds[1],bounds[3]))).first;

  const Mesh::Edge& e01 =*(this->GetEdges(*augmentedMesh).emplace(v0,v1)).first;
  const Mesh::Edge& e02 =*(this->GetEdges(*augmentedMesh).emplace(v0,v2)).first;
  const Mesh::Edge& e12 =*(this->GetEdges(*augmentedMesh).emplace(v1,v2)).first;
  const Mesh::Edge& e13 =*(this->GetEdges(*augmentedMesh).emplace(v1,v3)).first;
  const Mesh::Edge& e23 =*(this->GetEdges(*augmentedMesh).emplace(v2,v3)).first;

  this->GetTriangles(*augmentedMesh).emplace(e01,e02,e12);
  this->GetTriangles(*augmentedMesh).emplace(e13,e23,e12);

  InsertEdge insertEdge;

  Shape::PointVector vec;
  std::pair<const Mesh::Edge*, bool> firstEdge;
  for (unsigned i = 0; i < polygon.GetPoints().size(); ++i)
  {
    unsigned ipp = (i+1)%polygon.GetPoints().size();
    Shape::LineSegment l(polygon.GetPoints()[i], polygon.GetPoints()[ipp]);
    const Mesh::Edge* edge = insertEdge(l, mesh);
    vec.push_back(std::cref(static_cast<const Shape::Point&>(
			      edge->A() == polygon.GetPoints()[i] ? edge->A() :
			      edge->B())));
    if (i == 0)
    {
      bool isCCW =
	Shape::Dot(edge->B() - edge->A(),
		   polygon.GetPoints()[ipp] - polygon.GetPoints()[i]) > 0.;
      firstEdge = std::make_pair(edge, isCCW);
    }
  }

  RemoveBoundedRegion removeBoundedRegion;
  removeBoundedRegion(*firstEdge.first, !firstEdge.second, *augmentedMesh);

  if (inSitu)
  {
    this->GetPerimeter(mesh).SetPoints(vec);
  }
  else
  {
    for (auto& triangle : mesh_.GetTriangles())
    {
      const Mesh::Vertex& a =
	*(this->GetVertices(mesh).emplace(triangle.AB().A())).first;
      const Mesh::Vertex& b =
	*(this->GetVertices(mesh).emplace(triangle.AB().B())).first;
      const Mesh::Vertex& c =
	*(this->GetVertices(mesh).emplace(triangle.AC().B())).first;
      const Mesh::Edge& ab = *(this->GetEdges(mesh).emplace(a,b)).first;
      const Mesh::Edge& bc = *(this->GetEdges(mesh).emplace(b,c)).first;
      const Mesh::Edge& ac = *(this->GetEdges(mesh).emplace(a,c)).first;
      this->GetTriangles(mesh).emplace(ab,bc,ac);
    }
  }
}

}
}
