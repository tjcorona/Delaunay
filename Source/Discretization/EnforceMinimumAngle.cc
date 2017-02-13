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

#include "Discretization/EnforceMinimumAngle.hh"

#include "Discretization/AddInteriorPoint.hh"
#include "Discretization/SplitEdge.hh"
#include "Shape/CircleUtilities.hh"
#include "Shape/LineSegmentUtilities.hh"

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>

namespace Delaunay
{
namespace Discretization
{

void EnforceMinimumAngle::operator()(double angle, Delaunay::Mesh::Mesh& mesh) const
{
  assert(angle > 0.);

  std::set<const Mesh::Edge*> encroached;
  for (auto& edge : mesh.GetEdges())
  {
    if (IsEncroached(edge))
    {
      encroached.insert(&edge);
    }
  }

  SplitEdge splitEdge;
  AddInteriorPoint addInteriorPoint;
  bool allLegal = false;
  while (!allLegal)
  {
    allLegal = true;

    for (auto& edge : mesh.GetEdges())
    {
      if (encroached.find(&edge) != encroached.end())
      {
	allLegal = false;
	encroached.erase(&edge);
  	splitEdge(edge, mesh);
  	break;
      }
    }
    if (!allLegal)
      continue;

    for (auto& triangle : mesh.GetTriangles())
    {
      std::cout<<"minimum angle: "<<MinimumAngle(triangle)*180./M_PI<<std::endl;
      if (MinimumAngle(triangle) < angle*M_PI/180.)
      {
	allLegal = false;
	std::set<const Mesh::Edge*> encroachedByTriangle(
	  std::move(Encroaches(triangle.circumcircle.Center, mesh)));
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
  const Shape::Point& p, const Delaunay::Mesh::Mesh& mesh) const
{
  std::set<const Mesh::Edge*> encroached;
  for (auto& edge : mesh.GetEdges())
  {
    Shape::Circle c((edge.A() + edge.B())/2., Shape::Length(edge)/2.);

    if (Shape::Contains(c,p))
      encroached.insert(&edge);
  }
  return encroached;
}

void EnforceMinimumAngle::RecursivelySplitEdge(
  const Mesh::Edge& edge, Delaunay::Mesh::Mesh& mesh) const
{
  SplitEdge splitEdge;
  auto splitEdges = splitEdge(edge, mesh);

  if (IsEncroached(*splitEdges.first))
    RecursivelySplitEdge(*splitEdges.first, mesh);
  if (IsEncroached(*splitEdges.second))
    RecursivelySplitEdge(*splitEdges.second, mesh);
}

}
}
