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

#define _USE_MATH_DEFINES
#include <array>
#include <cassert>
#include <cmath>
#include <limits>

#include "Discretization/EnforceMinimumAngle.hh"

#include "Discretization/AddInteriorPoint.hh"
#include "Discretization/SplitEdge.hh"
#include "Mesh/Triangle.hh"
#include "Shape/CircleUtilities.hh"
#include "Shape/LineSegmentUtilities.hh"
#include "Shape/TriangleUtilities.hh"

namespace Delaunay
{
namespace Discretization
{

void EnforceMinimumAngle::operator()(double angle, Delaunay::Mesh::Mesh& mesh) const
{
  assert(angle > 0.);

  std::set<const Mesh::Edge*> boundaryEdges;
  double maximumLength = 0.;
  for (auto& edge : mesh.GetEdges())
  {
    double length = Length(edge);
    if (length > maximumLength)
      maximumLength = length;

    if (edge.boundary)
      boundaryEdges.insert(&edge);
  }

  SplitEdge splitEdge;
  AddInteriorPoint addInteriorPoint;
  bool allLegal = false;
  while (!allLegal)
  {
    allLegal = true;

    for (auto& edge : boundaryEdges)
    {
      if (IsEncroached(*edge))
      {
        if (SplitBoundaryEdge(*edge, boundaryEdges, mesh))
        {
          allLegal = false;
          break;
        }
      }
    }
    if (!allLegal)
      continue;

    for (auto& triangle : mesh.GetTriangles())
    {
      if (MinimumAngle(triangle) < angle*M_PI/180.)
      {
	std::set<const Mesh::Edge*> encroachedByTriangle(
	  Encroaches(triangle.circumcircle.Center, boundaryEdges));
	if (encroachedByTriangle.empty())
        {
          if (addInteriorPoint(triangle.circumcircle.Center, triangle, mesh))
          {
            allLegal = false;
          }
        }
	else
	{
	  for (const Mesh::Edge* edge : encroachedByTriangle)
          {
            if (edge->boundary)
            {
              if (SplitBoundaryEdge(*edge, boundaryEdges, mesh))
              {
                allLegal = false;
              }
            }
            else
            {
              splitEdge(*edge, mesh);
              allLegal = false;
            }
          }
	}
        if (allLegal == false)
          break;
      }
    }
  }
}

bool EnforceMinimumAngle::SplitBoundaryEdge(
  const Mesh::Edge& edge, std::set<const Mesh::Edge*>& boundaryEdges, Mesh::Mesh& mesh) const
{
  assert(edge.boundary);
  assert(edge.triangles.size() == 1);

  const Mesh::Triangle& t = **edge.triangles.begin();

  // Bail if the triangle's shortest edge is seditious
  {
    const Mesh::Edge* edges[3] = {&t.AB(), &t.BC(), &t.AC()};
    const Mesh::Vertex* vertices[3] = { &t.C(), &t.A(), &t.B() };
    double lengths[3] = {Shape::Length(*edges[0]), Shape::Length(*edges[1]),
                         Shape::Length(*edges[2])};

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

    if (edges[c]->boundary && edges[a]->boundary)
    {
      double angle = std::acos((lengths[a]*lengths[a] + lengths[c]*lengths[c] -
                                lengths[b]*lengths[b])/(2.*lengths[a]*lengths[c]));
      if (angle < M_PI/3.)
      {
        bool evenSplitA = false;
        for (const auto& e: vertices[a]->edges)
        {
          if (e != edges[a] && e->boundary && fabs(Shape::Length(*e) - lengths[a]) < EPSILON)
            evenSplitA = true;
        }

        bool evenSplitC = false;
        for (const auto& e: vertices[c]->edges)
        {
          if (e != edges[c] && e->boundary && fabs(Shape::Length(*e) - lengths[c]) < EPSILON)
            evenSplitC = true;
        }
        if (evenSplitA || evenSplitC)
        {
          return false;
        }
      }
    }
  }

  bool splitUnevenly[2] = { false, false };
  std::array<const Mesh::Vertex*, 2> v = { &edge.A(), &edge.B() };

  const Mesh::Vertex* w = &t.A();
  if (w == v[0] || w == v[1])
    w = &t.B();
  if (w == v[0] || w == v[1])
    w = &t.C();

  int orientation = Shape::Orientation(*v[0], *v[1], *w);
  // TODO: I shouldn't have orientation == 0 cases! Need to prevent seditious
  //       edges
  // assert(orientation != 0);
  if (orientation == 0)
    return false;

  if (orientation == 1)
    std::swap(v[0], v[1]);

  for (std::size_t i = 0; i < 2; i++)
  {
    for (const auto& e : v[i]->edges)
    {
      if (e == &edge)
        continue;

      if (e->boundary == false)
        continue;

      const Mesh::Vertex* v2 = &e->A();
      if (v2 == v[i])
        v2 = &e->B();

      double angle;
      if (i == 0)
        angle = Shape::Angle(*v2, *v[0], *v[1]);
      else
        angle = Shape::Angle(*v[0], *v[1], *v2);


      if (Shape::Angle(*v[(i+1)%2], *v[i], *v2) >= M_PI/2.)
        continue;

      splitUnevenly[i] = true;
    }
  }

  boundaryEdges.erase(&edge);

  if (splitUnevenly[0] != splitUnevenly[1])
  {
    double length = Shape::Length(edge);
    int i = std::round(std::log2(length/2.));

    double newLength = std::pow(2,i);
    double fraction = splitUnevenly[0] ? newLength/length : 1. - newLength/length;

    std::pair<const Mesh::Edge*, const Mesh::Edge*> newEdges = SplitEdge()(edge, fraction, mesh);

    boundaryEdges.insert(newEdges.first);
    boundaryEdges.insert(newEdges.second);
  }
  else if (splitUnevenly[0]) // && splitUnvenly[1]; implied
  {
    double length = Shape::Length(edge);
    int i = std::round(std::log2(length*.375));

    double newLength = std::pow(2,i);
    double fraction = newLength/length;

    std::pair<const Mesh::Edge*, const Mesh::Edge*> newEdges = SplitEdge()(edge, fraction, mesh);
    boundaryEdges.insert(newEdges.first);

    length = Shape::Length(*newEdges.second);
    i = std::round(std::log2(length/2.));

    newLength = std::pow(2,i);
    fraction = 1. - newLength/length;

    newEdges = SplitEdge()(*newEdges.second, fraction, mesh);

    boundaryEdges.insert(newEdges.first);
    boundaryEdges.insert(newEdges.second);
  }
  else
  {
    std::pair<const Mesh::Edge*, const Mesh::Edge*> newEdges = SplitEdge()(edge, mesh);
    boundaryEdges.insert(newEdges.first);
    boundaryEdges.insert(newEdges.second);
  }

  return true;
}

double EnforceMinimumAngle::MinimumAngle(const Mesh::Triangle& t) const
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

bool EnforceMinimumAngle::IsEncroached(const Mesh::Edge& e) const
{
  Shape::Circle c((e.A() + e.B())/2., Shape::Length(e)/2.);

  for (auto& triangle : e.triangles)
  {
    assert(triangle);
    const Mesh::Vertex* v = &triangle->AB().A();
    if (v == &e.A() || v == &e.B())
      v = &triangle->AB().B();
    if (v == &e.A() || v == &e.B())
      v = &triangle->AC().B();

    if (Shape::Contains(c,*v))
      return true;
  }
  return false;
}

std::set<const Mesh::Edge*> EnforceMinimumAngle::Encroaches(
  const Shape::Point& p, const std::set<const Mesh::Edge*>& edges) const
{
  std::set<const Mesh::Edge*> encroached;
  for (auto edge : edges)
  {
    Shape::Circle c((edge->A() + edge->B())/2., Shape::Length(*edge)/2.);

    if (Shape::Contains(c,p))
      encroached.insert(edge);
  }
  return encroached;
}

}
}
