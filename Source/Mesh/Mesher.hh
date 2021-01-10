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

#ifndef DELAUNAY_MESH_MESHER_HH
#define DELAUNAY_MESH_MESHER_HH

#include <vector>

#include "Mesh/Export.hh"

#include "Mesh/Mesh.hh"

namespace Delaunay
{
namespace Mesh
{
class DELAUNAYMESH_EXPORT Mesher
{
public:

  virtual ~Mesher() {}

  Mesh::ExteriorBoundary& GetPerimeter(Mesh& mesh) const
  { return mesh.Perimeter; }
  Mesh::InteriorBoundarySet& GetInteriorBoundaries(Mesh& mesh) const
    { return mesh.InteriorBoundaries; }
  Mesh::VertexSet&     GetVertices(Mesh& mesh)  const { return mesh.Vertices;  }
  Mesh::EdgeSet&       GetEdges(Mesh& mesh)     const { return mesh.Edges;     }
  Mesh::TriangleSet&   GetTriangles(Mesh& mesh) const { return mesh.Triangles; }

  std::pair<Mesh::VertexSet::const_iterator,bool>
  InsertVertex(const Shape::Point&, Mesh&);

  std::pair<Mesh::EdgeSet::const_iterator,bool>
  InsertEdge(const Vertex&, const Vertex&, Mesh&);

  std::pair<Mesh::TriangleSet::const_iterator,bool>
  InsertTriangle(const Edge&, const Edge&, const Edge&, Mesh&);
};

}
}

#endif
