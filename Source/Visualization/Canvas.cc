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

#include "Canvas.hh"

#include "Mesh/Edge.hh"
#include "Mesh/Mesh.hh"
#include "Mesh/Triangle.hh"

#include "Shape/LineSegment.hh"
#include "Shape/Point.hh"
#include "Shape/PointUtilities.hh"
#include "Shape/Triangle.hh"
#include "Shape/Polygon.hh"
#include "Shape/ParametricCurve.hh"

#include <unordered_map>

using namespace Delaunay::Misc;
using namespace Delaunay::Shape;

namespace
{
std::set<const Delaunay::Mesh::Triangle*> neighbors(const Delaunay::Mesh::Triangle& triangle)
{
  std::set<const Delaunay::Mesh::Triangle*> n;
  std::array<const Delaunay::Mesh::Edge*, 3> edges =
    { &triangle.AB(), &triangle.AC(), &triangle.BC() };
  for (auto edge : edges)
    for (auto& t : edge->triangles)
      if (t != &triangle)
        n.insert(t);
  return n;
}
}

namespace Delaunay
{
namespace Visualization
{
void Canvas::Draw(const Mesh::Mesh& mesh, const std::array<Color, 4>& triangleColors)
{
  std::unordered_map<Mesh::TriangleSet::value_type, int> colorAssignments;

  colorAssignments.reserve(mesh.GetTriangles().size());
  int counter = 0;
  for (auto& triangle : mesh.GetTriangles())
  {
    colorAssignments[&triangle] = counter++;

    if (counter == static_cast<int>(triangleColors.size()))
      counter = 0;
  }

  bool assigned = false;

  while (!assigned)
  {
    assigned = true;

    for (auto it = colorAssignments.begin(); it != colorAssignments.end(); ++it)
    {
      int color = it->second;
      std::array<int, 3> assignedValues = { -1, -1, -1 };
      counter = 0;
      for (auto& neighbor : neighbors(*(it->first)))
      {
        assignedValues[counter++] = colorAssignments[neighbor];
      }

      while (color == assignedValues[0] ||
             color == assignedValues[1] || color == assignedValues[2])
      {
        assigned = false;
        color = (color + 1) % triangleColors.size();
      }
      colorAssignments[it->first] = color;
    }
  }

  for (auto it = colorAssignments.begin(); it != colorAssignments.end(); ++it)
    Draw(*static_cast<const Delaunay::Shape::Triangle*>(it->first),
         Visualization::Black, triangleColors[it->second]);
}
}
}
