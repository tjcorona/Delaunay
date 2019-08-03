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
	allLegal = false;

	boundaryEdges.erase(edge);
  	std::pair<const Mesh::Edge*, const Mesh::Edge*> newEdges = splitEdge(*edge, mesh);
        boundaryEdges.insert(newEdges.first);
        boundaryEdges.insert(newEdges.second);
  	break;
      }
    }
    if (!allLegal)
      continue;

    for (auto& triangle : mesh.GetTriangles())
    {
      if (MinimumAngle(triangle) < angle*M_PI/180. && Shape::Area(triangle) > 1.e-6)
      {
	allLegal = false;
	std::set<const Mesh::Edge*> encroachedByTriangle(
	  Encroaches(triangle.circumcircle.Center, boundaryEdges));
	if (encroachedByTriangle.empty())
	  addInteriorPoint(triangle.circumcircle.Center, mesh);
	else
	{
	  for (auto& edge : mesh.GetEdges())
	  {
	    if (encroachedByTriangle.find(&edge) != encroachedByTriangle.end())
	    {
	      splitEdge(edge, mesh);
	      break;
	    }
	  }
	}
	break;
      }
    }
  }
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
