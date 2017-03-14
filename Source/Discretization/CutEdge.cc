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

#include "CutEdge.hh"

#include "Shape/LineSegment.hh"
#include "Shape/LineSegmentUtilities.hh"
#include "Shape/Triangle.hh"
#include "Shape/TriangleUtilities.hh"

#include <cassert>

namespace Delaunay
{
namespace Discretization
{

std::set<const Mesh::Edge*> CutEdge::operator()(
  const Shape::LineSegment& l, Mesh::Mesh& mesh)
{
  const Mesh::Triangle* t1 = mesh.FindContainingTriangle(l.A);
  const Mesh::Triangle* t2 = mesh.FindContainingTriangle(l.B);

  if (!t1 || !t2)
  {
    return std::set<const Mesh::Edge*>();
  }

  assert(t1 && t2);

  std::set<const Mesh::Edge*> insertedEdges;

  std::set<const Mesh::Triangle*> intersected(
    this->FindContainingTriangles(l, mesh));

  for (auto triangle : intersected)
  {
    const Mesh::Edge* edge = this->CutEdgeInTriangle(l, *triangle, mesh);

    if (edge != nullptr)
      insertedEdges.insert(edge);
  }

  for (auto triangle : this->trianglesToRemove)
    this->GetTriangles(mesh).erase(*triangle);
  this->trianglesToRemove.clear();

  for (auto edge : this->edgesToRemove)
    this->GetEdges(mesh).erase(*edge);
  this->edgesToRemove.clear();

  return insertedEdges;
}

namespace
{
bool NeedsToSplit(const Shape::LineSegment& l, const Shape::Triangle& t)
{
  return (Shape::Intersect(l, t) || Shape::Contains(t, l) ||
          Shape::Coincident(t, l.A) || Shape::Coincident(t, l.B));
}
}

std::set<const Mesh::Triangle*> CutEdge::FindContainingTriangles(
  const Shape::LineSegment& l, Delaunay::Mesh::Mesh& mesh) const
{
  const Mesh::Triangle* t1 = mesh.FindContainingTriangle(l.A);

  std::set<const Mesh::Triangle*> intersected;

  if (NeedsToSplit(l, *t1))
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

      if (NeedsToSplit(l, *t))
      {
        intersected.insert(t);
        toSearch.insert(t);
      }
    }

    for (auto t : triangle->B().triangles)
    {
      if (searched.find(t) != searched.end())
        continue;

      if (NeedsToSplit(l, *t))
      {
        intersected.insert(t);
        toSearch.insert(t);
      }
    }

    for (auto t : triangle->C().triangles)
    {
      if (searched.find(t) != searched.end())
        continue;

      if (NeedsToSplit(l, *t))
      {
        intersected.insert(t);
        toSearch.insert(t);
      }
    }
  }

  return intersected;
}

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

const Mesh::Edge* CutEdge::CutEdgeInTriangle(const Shape::LineSegment& l,
						   const Mesh::Triangle& t,
						   Mesh::Mesh& mesh)
{
  // possible scenarios:
  // 1.  the line segment is contained entirely within the triangle
  // 2a. one end of the line segment is contained entirely within the triangle,
  //     and the line segment crosses an edge
  // 2b. one end of the line segment is contained entirely within the triangle,
  //     and the line segment crosses a vertex
  // 3a. the line segment crosses two edges
  // 3b. the line segment crosses one edge and one vertex

  bool triContainsA = Contains(t,l.A) && !Coincident(t,l.A);
  bool triContainsB = Contains(t,l.B) && !Coincident(t,l.B);

  if (triContainsA && triContainsB)
  {
    // 1.  the line segment is contained entirely within the triangle

    const Mesh::Vertex* lA = &(*(this->GetVertices(mesh).emplace(l.A)).first);
    const Mesh::Vertex* lB = &(*(this->GetVertices(mesh).emplace(l.B)).first);

    const Mesh::Vertex* p1 = &t.AB().A();
    const Mesh::Vertex* p2 = &t.AB().B();
    const Mesh::Vertex* p3 = &t.AC().B();

    if (Shape::Orientation(*lA, *lB, *p1) != -1 ||
        Shape::Orientation(*lA, *lB, *p2) != -1)
    {
      const Mesh::Vertex* tmp = lA;
      lA = lB;
      lB = tmp;
    }

    const Mesh::Edge& e1 = *(this->GetEdges(mesh).emplace(*lA, *lB)).first;
    const Mesh::Edge& e2 = *(this->GetEdges(mesh).emplace(*lA, *p3)).first;
    const Mesh::Edge& e3 = *(this->GetEdges(mesh).emplace(*lB, *p3)).first;
    const Mesh::Edge& e4 = *(this->GetEdges(mesh).emplace(*lA, *p1)).first;
    const Mesh::Edge& e5 = *(this->GetEdges(mesh).emplace(*lB, *p2)).first;
    const Mesh::Edge& e6 = *(this->GetEdges(mesh).emplace(*lB, *p1)).first;

    this->GetTriangles(mesh).emplace(e1, e2, e3);
    this->GetTriangles(mesh).emplace(*GetEdge(p1, p3), e2, e4);
    this->GetTriangles(mesh).emplace(*GetEdge(p2, p3), e3, e5);
    this->GetTriangles(mesh).emplace(e1, e4, e6);
    this->GetTriangles(mesh).emplace(*GetEdge(p1, p2), e6, e5);

    this->trianglesToRemove.insert(&t);

    return &e1;
  }
  else if (triContainsA || triContainsB)
  {
    // 2a. one end of the line segment is contained entirely within the
    //     triangle, and the line segment crosses an edge
    // 2b. one end of the line segment is contained entirely within the
    //     triangle, and the line segment crosses a vertex

    const Shape::Point* p1 = (triContainsA ? &l.A : &l.B);
    const Shape::Point* p2 = (triContainsA ? &l.B : &l.A);

    const Mesh::Edge* toSplit = &t.AB();
    const Mesh::Vertex* opposing = &t.AC().B();
    if (Shape::IntersectOrCoincident(l,*toSplit) == false)
    {
      toSplit = &t.BC();
      opposing = &t.AB().A();
    }
    if (Shape::IntersectOrCoincident(l,*toSplit) == false)
    {
      toSplit = &t.AC();
      opposing = &t.AB().B();
    }

    const Mesh::Vertex& vtx1 = *(this->GetVertices(mesh).emplace(*p1)).first;
    const Shape::Point intersection = Intersection(l, *toSplit);

    if (intersection == toSplit->A() || intersection == toSplit->B())
    {
      const Mesh::Vertex& vtx2 = (intersection == toSplit->A() ?
                                  toSplit->A() : toSplit->B());
      const Mesh::Edge& e1 = *(this->GetEdges(mesh).emplace(vtx1, vtx2)).first;

      const Mesh::Vertex* opposing2 = &toSplit->A();

      if (vtx2 == toSplit->A())
        opposing2 = &toSplit->B();

      const Mesh::Edge& e2 =
        *(this->GetEdges(mesh).emplace(vtx1, *opposing)).first;
      const Mesh::Edge& e3 =
        *(this->GetEdges(mesh).emplace(vtx1, *opposing2)).first;

      this->GetTriangles(mesh).emplace(*GetEdge(opposing, &vtx2), e1, e2);
      this->GetTriangles(mesh).emplace(*GetEdge(opposing2, &vtx2), e1, e3);
      this->GetTriangles(mesh).emplace(*GetEdge(opposing, opposing2), e2, e3);

      this->trianglesToRemove.insert(&t);

      return &e1;
    }

    const Mesh::Vertex& vtx2 =
      *(this->GetVertices(mesh).emplace(intersection)).first;
    const Mesh::Edge& e1 = *(this->GetEdges(mesh).emplace(vtx1, vtx2)).first;
    const Mesh::Edge& e2 =
      *(this->GetEdges(mesh).emplace(vtx2, toSplit->A())).first;
    const Mesh::Edge& e3 =
      *(this->GetEdges(mesh).emplace(vtx2, toSplit->B())).first;
    const Mesh::Edge& e4 =
      *(this->GetEdges(mesh).emplace(vtx1, toSplit->A())).first;
    const Mesh::Edge& e5 =
      *(this->GetEdges(mesh).emplace(vtx1, toSplit->B())).first;
    const Mesh::Edge& e6 =
      *(this->GetEdges(mesh).emplace(vtx1, *opposing)).first;

    this->GetTriangles(mesh).emplace(e1, e2, e4);
    this->GetTriangles(mesh).emplace(e1, e3, e5);
    this->GetTriangles(mesh).emplace(e4, e6, *GetEdge(&toSplit->A(), opposing));
    this->GetTriangles(mesh).emplace(e5, e6, *GetEdge(&toSplit->B(), opposing));

    this->trianglesToRemove.insert(&t);
    this->edgesToRemove.insert(toSplit);

    return &e1;
  }

  // 3a. the line segment crosses two edges
  // 3b. the line segment crosses one edge and one vertex
  // 3c. the line segment touches an edge

  auto intersections = Intersection(l,t);

  if (std::get<0>(intersections) == 1)
  {
    if (std::get<1>(intersections) == t.AB().A() ||
	std::get<1>(intersections) == t.AB().B() ||
	std::get<1>(intersections) == t.AC().B())
      return nullptr;

    const Mesh::Vertex& vtx =
      *(this->GetVertices(mesh).emplace(std::get<1>(intersections))).first;

    const Mesh::Edge* toSplit;
    const Mesh::Vertex* opposite;

    if (Shape::Contains(t.AB(), vtx))
    {
      toSplit = &t.AB();
      opposite = &t.AC().B();
    }
    else if (Shape::Contains(t.BC(), vtx))
    {
      toSplit = &t.BC();
      opposite = &t.AB().A();
    }
    else
    {
      toSplit = &t.AC();
      opposite = &t.AB().B();
    }

    const Mesh::Edge& e1 =
      *(this->GetEdges(mesh).emplace(vtx, *opposite)).first;
    const Mesh::Edge& e2 =
      *(this->GetEdges(mesh).emplace(toSplit->A(), vtx)).first;
    const Mesh::Edge& e3 =
      *(this->GetEdges(mesh).emplace(toSplit->B(), vtx)).first;

    this->GetTriangles(mesh).emplace(e1, e2, *GetEdge(opposite, &toSplit->A()));
    this->GetTriangles(mesh).emplace(e1, e3, *GetEdge(opposite, &toSplit->B()));

    this->trianglesToRemove.insert(&t);
    this->edgesToRemove.insert(toSplit);

    return nullptr;
  }

  assert(std::get<0>(intersections) == 2);

  const Shape::Point& pt1 = std::get<1>(intersections);
  const Shape::Point& pt2 = std::get<2>(intersections);
  const Mesh::Edge* toSplit[2];
  const Mesh::Edge* triEdges[4] = {&t.AB(), &t.BC(), &t.AC(), nullptr};


  toSplit[0] = (Shape::Contains(t.AB(), pt1) ? &t.AB() :
                Shape::Contains(t.BC(), pt1) ? &t.BC() :
                Shape::Contains(t.AC(), pt1) ? &t.AC() : nullptr);
  toSplit[1] = (Shape::Contains(t.AB(), pt2) ? &t.AB() :
                Shape::Contains(t.BC(), pt2) ? &t.BC() :
                Shape::Contains(t.AC(), pt2) ? &t.AC() : nullptr);

  assert(toSplit[0] != nullptr);

  const Mesh::Vertex& vtx1 = (pt1 == toSplit[0]->A() ? toSplit[0]->A() :
			      pt1 == toSplit[0]->B() ? toSplit[0]->B() :
			      *(this->GetVertices(mesh).emplace(pt1)).first);
  const Mesh::Vertex& vtx2 = (pt2 == toSplit[1]->A() ? toSplit[1]->A() :
			      pt2 == toSplit[1]->B() ? toSplit[1]->B() :
			      *(this->GetVertices(mesh).emplace(pt2)).first);

  if (vtx1 == toSplit[0]->A() || vtx1 == toSplit[0]->B() ||
      vtx2 == toSplit[1]->A() || vtx2 == toSplit[1]->B())
  {
    if ((vtx1 == toSplit[0]->A() || vtx1 == toSplit[0]->B()) &&
        (vtx2 == toSplit[1]->A() || vtx2 == toSplit[1]->B()))
      return GetEdge(&vtx1,&vtx2);

    const Mesh::Vertex* v, *vSplit;
    const Mesh::Edge* split;
    if (vtx1 == toSplit[0]->A() || vtx1 == toSplit[0]->B())
    {
      v = &vtx1;
      vSplit = &vtx2;
      split = toSplit[1];
    }
    else
    {
      v = &vtx2;
      vSplit = &vtx1;
      split = toSplit[0];
    }

    const Mesh::Vertex* v1, *v2;
    if (Orientation(*v,*vSplit, split->A()) == 1)
    {
      v1 = &split->A();
      v2 = &split->B();
    }
    else
    {
      v1 = &split->B();
      v2 = &split->A();
    }

    const Mesh::Edge& e1 = *(this->GetEdges(mesh).emplace(*v, *vSplit)).first;
    const Mesh::Edge& e2 = *(this->GetEdges(mesh).emplace(*vSplit, *v1)).first;
    const Mesh::Edge& e3 = *(this->GetEdges(mesh).emplace(*vSplit, *v2)).first;

    this->GetTriangles(mesh).emplace(e1, e2, *GetEdge(v, v1));
    this->GetTriangles(mesh).emplace(e1, e3, *GetEdge(v, v2));

    this->trianglesToRemove.insert(&t);
    this->edgesToRemove.insert(split);

    return &e1;
  }

  assert(toSplit[0] != toSplit[1]);
  assert(toSplit[1] != nullptr);

  const Mesh::Vertex* common = &(toSplit[0]->A());
  const Mesh::Vertex* vtx1p = &(toSplit[0]->B());
  if (common != &(toSplit[1]->A()) && common != &(toSplit[1]->B()))
  {
    common = &(toSplit[0]->B());
    vtx1p = &(toSplit[0]->A());
  }

  const Mesh::Vertex* vtx2p = (common == &(toSplit[1]->B()) ?
                               &(toSplit[1]->A()) : &(toSplit[1]->B()));

  assert(common == &(toSplit[1]->A()) || common == &(toSplit[1]->B()));

  const Mesh::Edge& e1 = *(this->GetEdges(mesh).emplace(vtx1, vtx2)).first;
  const Mesh::Edge& e2 = *(this->GetEdges(mesh).emplace(vtx1, *vtx1p)).first;
  const Mesh::Edge& e3 = *(this->GetEdges(mesh).emplace(vtx2, *vtx2p)).first;
  const Mesh::Edge& e4 = *(this->GetEdges(mesh).emplace(vtx1, *common)).first;
  const Mesh::Edge& e5 = *(this->GetEdges(mesh).emplace(vtx2, *common)).first;
  const Mesh::Edge& e6 = *(this->GetEdges(mesh).emplace(*vtx1p, vtx2)).first;

  this->GetTriangles(mesh).emplace(e1, e4, e5);
  this->GetTriangles(mesh).emplace(e1, e2, e6);
  this->GetTriangles(mesh).emplace(*GetEdge(vtx1p,vtx2p), e3, e6);

  this->trianglesToRemove.insert(&t);
  this->edgesToRemove.insert(toSplit[0]);
  this->edgesToRemove.insert(toSplit[1]);

  return &e1;
}

}
}
