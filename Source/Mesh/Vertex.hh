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

#ifndef DELAUNAY_MESH_VERTEX_HH
#define DELAUNAY_MESH_VERTEX_HH

#include <set>

#include "Mesh/Export.hh"

#include "Shape/Point.hh"

#include "Mesh/EdgeSet.hh"
#include "Mesh/TriangleSet.hh"

namespace Delaunay
{
namespace Mesh
{

class DELAUNAYMESH_EXPORT Vertex : public Shape::Point
{
public:
  Vertex(const Shape::Point& p) : Point(p) {}
  Vertex(double x,double y) : Point(x,y) {}
  virtual ~Vertex() = default;

  mutable TriangleSet triangles;
  mutable EdgeSet edges;
};

typedef std::list<std::reference_wrapper<const Vertex> > VertexList;

}
}

#endif
