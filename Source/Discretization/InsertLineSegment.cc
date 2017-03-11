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

#include "InsertLineSegment.hh"

// #define USE_CDM 1

#include "Discretization/AddInteriorPoint.hh"
#if USE_CDM
#include "Discretization/ConstrainedDelaunayMesh.hh"
#else
#include "Discretization/DiscretizePolygon.hh"
#endif
#include "Discretization/RemoveBoundedRegion.hh"
#include "Shape/LineSegment.hh"
#include "Shape/LineSegmentUtilities.hh"
#include "Shape/Triangle.hh"
#include "Shape/TriangleUtilities.hh"

#include <array>
#include <cassert>
#include <map>
#include <vector>
#include <utility>

namespace Delaunay
{
namespace Discretization
{

namespace
{
const Mesh::Edge* GetEdge(const Mesh::Vertex* v1, const Mesh::Vertex* v2)
{
  for (auto edge : v1->edges)
    if (&edge->A() == v2 || &edge->B() == v2)
      return edge;

  return nullptr;
}
}

const Mesh::Edge* InsertLineSegment::operator()(const Shape::LineSegment& l,
                                         Mesh::Mesh& mesh)
{
// TODO: deal with the case where l lies on preexisting edges (Touches() returns// false, so 0 triangles are described as between the two points in this case)

  // First, add the two points of the line segment
  AddInteriorPoint addInteriorPoint;
  const Mesh::Vertex* v1 = addInteriorPoint(l.A, mesh);
  const Mesh::Vertex* v2 = addInteriorPoint(l.B, mesh);

  // If there is already present, mark it as a boundary and return
  if (const Mesh::Edge* edge = GetEdge(v1,v2))
  {
    const_cast<Mesh::Edge*>(edge)->boundary = true;
    return edge;
  }

  // Collect all of the triangles that contain the line
  std::set<const Mesh::Triangle*> intersected(
    std::move(this->FindContainingTriangles(l, mesh)));

  // Construct the containing polygon as the union of the containing triangles,
  // and mark the polygon edges as boundary edges
  std::set<Mesh::Edge*> temporaryBoundaries;
  Shape::Polygon poly(std::move(this->PolygonFromTriangleSet(intersected)));
  for (std::size_t i=0;i<poly.GetPoints().size();i++)
  {
    std::size_t ipp = (i+1)%poly.GetPoints().size();
    const Mesh::Edge* edge =
      GetEdge(static_cast<const Mesh::Vertex*>(&poly.GetPoints()[i].get()),
              static_cast<const Mesh::Vertex*>(&poly.GetPoints()[ipp].get()));
    if (!edge->boundary)
    {
      Mesh::Edge* e = const_cast<Mesh::Edge*>(edge);
      e->boundary = true;
      temporaryBoundaries.insert(e);
    }
  }

  // Split the containing polygon along the edge we wish to insert
  std::pair<Shape::Polygon,Shape::Polygon> polys(
    std::move(BisectPolygon(poly, *v1, *v2)));

  // Identify the polygon's orientation
  const Mesh::Edge* edge =
    GetEdge(static_cast<const Mesh::Vertex*>(&poly.GetPoints()[0].get()),
            static_cast<const Mesh::Vertex*>(&poly.GetPoints()[1].get()));
  bool isCCW =
    Shape::Dot(edge->B() - edge->A(),
               poly.GetPoints()[1] - poly.GetPoints()[0]) > 0.;

  // Remove the containing polygon from the mesh
  RemoveBoundedRegion removeBoundedRegion;
  removeBoundedRegion(*edge, isCCW, mesh);

  // Insert the bisected polygons into the mesh
#if USE_CDM
  ConstrainedDelaunayMesh discretizePolygon;
#else
  DiscretizePolygon discretizePolygon;
#endif
  discretizePolygon(polys.first, mesh);
  discretizePolygon(polys.second, mesh);

  for (auto& edge : temporaryBoundaries)
  {
    edge->boundary = false;
  }

  return GetEdge(v1,v2);
}

Shape::Polygon InsertLineSegment::PolygonFromTriangleSet(
  const Mesh::TriangleSet& triangleSet) const
{
  typedef std::pair<const Mesh::Vertex*, const Mesh::Vertex*> OrderedEdge;
  typedef std::map<OrderedEdge,std::size_t> EdgeHistogram;
  typedef std::multimap<const Mesh::Vertex*, const Mesh::Vertex*> EdgeMap;

  EdgeHistogram edgeCounter;
  EdgeMap edges;

  for (auto& triangle : triangleSet)
  {
    std::array<OrderedEdge,3> triEdges;
    if (Shape::Orientation(triangle->AB().A(), triangle->AB().B(),
                           triangle->AC().B()) == 1)
    {
      triEdges[0] = std::make_pair(&triangle->AB().A(), &triangle->AB().B());
      triEdges[1] = std::make_pair(&triangle->AB().B(), &triangle->AC().B());
      triEdges[2] = std::make_pair(&triangle->AC().B(), &triangle->AB().A());
    }
    else
    {
      triEdges[0] = std::make_pair(&triangle->AB().A(), &triangle->AC().B());
      triEdges[1] = std::make_pair(&triangle->AC().B(), &triangle->AB().B());
      triEdges[2] = std::make_pair(&triangle->AB().B(), &triangle->AB().A());
    }

    for (auto& e : triEdges)
    {
      OrderedEdge eInv(e.second, e.first);

      ++(edgeCounter[e]);

      if (edgeCounter[eInv] == 0)
      {
        edges.insert(e);
      }
      else if (edgeCounter[e] == 1)
      {
        std::pair<EdgeMap::iterator,
                  EdgeMap::iterator> range = edges.equal_range(eInv.first);

        for (EdgeMap::iterator it = range.first; it != range.second; ++it)
        {
          if (it->second == eInv.second)
          {
            edges.erase(it);
            break;
          }
        }
      }
    }
  }

  assert(!edges.empty());

  Shape::PointVector polyVertices;

  EdgeMap::iterator edgeIt = edges.begin();
  OrderedEdge edge = *(edgeIt);

  const Mesh::Vertex* const firstVtx = edge.first;

  do
  {
    polyVertices.push_back(
      std::cref(static_cast<const Shape::Point&>(*edge.first)));
    edgeIt = edges.find(edge.second);
    edge = *(edgeIt);
    edges.erase(edgeIt);
  }
  while (edge.first != firstVtx);

  assert(edges.empty());

  return Shape::Polygon(polyVertices);
}

namespace
{
bool Touches(const Shape::LineSegment& l, const Shape::Triangle& t)
{
  return (Shape::Intersect(l, t) || Shape::Contains(t, l));
}
}

std::set<const Mesh::Triangle*> InsertLineSegment::FindContainingTriangles(
  const Shape::LineSegment& l, Delaunay::Mesh::Mesh& mesh) const
{
  const Mesh::Triangle* t1 = mesh.FindContainingTriangle(l.A);
  const Mesh::Triangle* t2 = mesh.FindContainingTriangle(l.B);

  std::set<const Mesh::Triangle*> intersected;

  if (Touches(l, *t1))
    intersected.insert(t1);

  std::set<const Mesh::Triangle*> toSearch;
  std::set<const Mesh::Triangle*> searched;

  toSearch.insert(t1);

  while (!toSearch.empty())
  {
    auto triIt = toSearch.begin();
    const Mesh::Triangle* triangle = *triIt;
    searched.insert(triangle);
    toSearch.erase(triIt);

    for (auto t : triangle->A().triangles)
    {
      if (searched.find(t) != searched.end())
        continue;

      if (Touches(l, *t))
      {
        intersected.insert(t);
        toSearch.insert(t);
      }
    }

    for (auto t : triangle->B().triangles)
    {
      if (searched.find(t) != searched.end())
        continue;

      if (Touches(l, *t))
      {
        intersected.insert(t);
        toSearch.insert(t);
      }
    }

    for (auto t : triangle->C().triangles)
    {
      if (searched.find(t) != searched.end())
        continue;

      if (Touches(l, *t))
      {
        intersected.insert(t);
        toSearch.insert(t);
      }
    }
  }

  return intersected;
}

std::pair<Shape::Polygon,Shape::Polygon> InsertLineSegment::BisectPolygon(
  const Shape::Polygon& p, const Mesh::Vertex& v1, const Mesh::Vertex& v2) const
{
  typedef std::reverse_iterator<Shape::PointVector::const_iterator> RIter;
  Shape::PointVector::const_iterator it1, it2;
  for (it1 = p.GetPoints().begin(); *it1 != v1; ++it1)
    assert(it1 != p.GetPoints().end());
  for (it2 = p.GetPoints().begin(); *it2 != v2; ++it2)
    assert(it2 != p.GetPoints().end());
  if (it1 > it2)
    std::swap(it1,it2);

  Shape::PointVector p1(it1,std::next(it2));
  Shape::PointVector p2(it2,p.GetPoints().end());
  p2.insert(p2.end(), p.GetPoints().begin(), std::next(it1));

  return std::make_pair(Shape::Polygon(p1),Shape::Polygon(p2));
}

}
}
