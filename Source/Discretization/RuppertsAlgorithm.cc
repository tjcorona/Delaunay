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

#include "RuppertsAlgorithm.hh"

#include "Discretization/AddInteriorPoint.hh"
#include "Discretization/RemoveBoundedRegion.hh"
#include "Discretization/SplitEdge.hh"
#include "Shape/CircleUtilities.hh"
#include "Shape/LineSegmentUtilities.hh"
#include "Shape/PointUtilities.hh"
#include "Shape/PolygonUtilities.hh"
#include "Shape/TriangleUtilities.hh"

#include <cassert>
#include <limits>

namespace Delaunay
{
namespace Discretization
{

namespace
{
const Mesh::Edge* GetEdge(const Mesh::Vertex& v1, const Mesh::Vertex& v2)
{
  for (auto edge : v1.edges)
    if (&edge->A() == &v2 || &edge->B() == &v2)
      return edge;

  return nullptr;
}
}

void RuppertsAlgorithm::operator()(
  const Delaunay::Shape::Polygon& polygon, Delaunay::Mesh::Mesh& mesh)
{
  // double alpha = 20.7;

  std::array<double, 4> bounds(Shape::Bounds(polygon));
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

  VertexList vertices;
  VertexList::const_iterator sentinel = vertices.before_begin();
  AddInteriorPoint addInteriorPoint;
  for (auto& p : polygon.GetPoints())
  {
    const Mesh::Vertex& vtx = *addInteriorPoint(p, *augmentedMesh);
    vertices.insert_after(sentinel, std::cref(vtx));
    std::advance(sentinel, 1);
  }

  bool isConverged = false;

  std::size_t counter = 0;
  while (!isConverged)
  {
    if (counter++ > 30)
      break;
    isConverged = true;

    for (VertexList::const_iterator vtx1 = vertices.begin();
	 vtx1 != vertices.end(); ++vtx1)
    {
      VertexList::const_iterator vtx2 = std::next(vtx1);
      if (vtx2 == vertices.end())
	vtx2 = vertices.begin();

      if (IsEncroached(vtx1, vtx2, *augmentedMesh))
      {
	isConverged = false;
	RecursivelySplitEdge(vtx1, vtx2, vertices, *augmentedMesh);
      }
    }

    for (auto& triangle : augmentedMesh->GetTriangles())
    {
      if (MinimumAngle(triangle) < 20.7)
      {
	isConverged = false;
	std::vector<VertexList::const_iterator> encroached(
	  Encroaches(triangle.circumcircle.Center, vertices));
	if (!encroached.empty())
	{
	  for (auto& vtx1 : encroached)
	  {
	    auto vtx2 = std::next(vtx1);
	    if (vtx2 == vertices.end())
	      vtx2 = vertices.begin();

	    RecursivelySplitEdge(vtx1, vtx2, vertices, *augmentedMesh);
	  }
	}
	else
	{
	  addInteriorPoint(triangle.circumcircle.Center, *augmentedMesh);
	}
	break;
      }
    }
  }

  Shape::PointList list;

  for (VertexList::const_iterator vtx1 = vertices.begin();
       vtx1 != vertices.end(); ++vtx1)
  {
    VertexList::const_iterator vtx2 = std::next(vtx1);
    if (vtx2 == vertices.end())
      vtx2 = vertices.begin();

    const_cast<Mesh::Edge*>(GetEdge(*vtx1, *vtx2))->boundary = true;
    list.push_back(std::cref(static_cast<const Shape::Point&>(*vtx1)));
  }

  RemoveBoundedRegion removeBoundedRegion;
  removeBoundedRegion(**(v0.triangles.begin()), *augmentedMesh);

  this->GetPerimeter(*augmentedMesh).SetPoints(list);

  if (!inSitu)
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

double RuppertsAlgorithm::MinimumAngle(const Mesh::Triangle& t) const
{
  const Mesh::Edge* edge[3] = {&t.AB(), &t.BC(), &t.AC()};
  double lengths[3] = {Shape::Length(*edge[0]), Shape::Length(*edge[1]),
                       Shape::Length(*edge[2])};

  std::size_t b = 0;
  for (std::size_t i=0; i<3; i++)
  {
    if (i == b)
      continue;

    if (lengths[i] < lengths[b])
      b = i;
  }
  std::size_t c = (b+1)%3;
  std::size_t a = (b+2)%3;

  if (edge[a]->boundary && edge[c]->boundary)
    return std::numeric_limits<double>::max();

  return std::acos((lengths[a]*lengths[a] + lengths[c]*lengths[c] -
                    lengths[b]*lengths[b])/(2.*lengths[a]*lengths[c]));
}

bool RuppertsAlgorithm::IsEncroached(const VertexList::const_iterator& vtx1,
				     const VertexList::const_iterator& vtx2,
				     const Mesh::Mesh&) const
{
  Shape::Circle c((*vtx1 + *vtx2)/2., Shape::Distance(*vtx2, *vtx1)/2.);

  const Mesh::Edge* edge = GetEdge(*vtx1, *vtx2);

  if (!edge)
    return true;

  for (auto& triangle : edge->triangles)
  {
    const Mesh::Vertex* v = &triangle->AB().A();
    if (*v == *vtx1 || *v == *vtx2)
      v = &triangle->AB().B();
    if (*v == *vtx1 || *v == *vtx2)
      v = &triangle->AC().B();

    assert(*v != *vtx1 && *v != *vtx2);

    if (Shape::Contains(c,*v))
      return true;
  }

  return false;
}

std::vector<RuppertsAlgorithm::VertexList::const_iterator>
RuppertsAlgorithm::Encroaches(const Shape::Point& p,
			      const VertexList& vertices) const
{
  std::vector<VertexList::const_iterator> encroached;

  for (VertexList::const_iterator vtx1 = vertices.begin();
       vtx1 != vertices.end(); ++vtx1)
  {
    VertexList::const_iterator vtx2 = std::next(vtx1);
    if (vtx2 == vertices.end())
      vtx2 = vertices.begin();

    Shape::LineSegment l(*vtx1, *vtx2);
    Shape::Circle c((*vtx1 + *vtx2)/2., Shape::Distance(*vtx1, *vtx2)/2.);

    if (Shape::Contains(c,p))
      encroached.push_back(vtx1);
  }
  return encroached;
}

void RuppertsAlgorithm::RecursivelySplitEdge(
  const VertexList::const_iterator& vtx1,
  const VertexList::const_iterator& vtx2,
  VertexList& vertices,
  Delaunay::Mesh::Mesh& mesh) const
{
  const Mesh::Vertex* v;

  if (const Mesh::Edge* edge = GetEdge(*vtx1, *vtx2))
  {
    SplitEdge splitEdge;
    auto edges = splitEdge(*edge, mesh);
    v = (&edges.first->A() == &edges.second->A() ||
	 &edges.first->A() == &edges.second->B() ?
	 &edges.first->A() : &edges.first->B());
  }
  else
  {
    AddInteriorPoint addInteriorPoint;
    v = addInteriorPoint((*vtx1 + *vtx2)/2., mesh);
  }

  VertexList::const_iterator vtx = vertices.insert_after(vtx1, *v);

  if (IsEncroached(vtx1, vtx, mesh))
  {
    RecursivelySplitEdge(vtx1, vtx, vertices, mesh);
  }
  if (IsEncroached(vtx, vtx2, mesh))
  {
    RecursivelySplitEdge(vtx, vtx2, vertices, mesh);
  }
}

const Mesh::Triangle* RuppertsAlgorithm::FindContainingTriangle(
  const Shape::Point& p, const Delaunay::Mesh::Mesh& mesh) const
{
  for (auto it = mesh.GetTriangles().begin();
       it != mesh.GetTriangles().end(); ++it)
    if (Contains(*it, p))
      return &(*it);

  return nullptr;
}

}
}
