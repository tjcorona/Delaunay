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

#include "Mesh/Mesh.hh"

namespace Delaunay
{
namespace Mesh
{
class Mesher
{
public:

  virtual ~Mesher() {}

  Mesh::Polygon&       GetPerimeter(Mesh& mesh) { return mesh.Perimeter; }
  Mesh::InteriorBoundarySet& GetInteriorBoundaries(Mesh& mesh)
    { return mesh.InteriorBoundaries; }
  Mesh::VertexSet&     GetVertices(Mesh& mesh)  { return mesh.Vertices;  }
  Mesh::EdgeSet&       GetEdges(Mesh& mesh)     { return mesh.Edges;     }
  Mesh::TriangleSet&   GetTriangles(Mesh& mesh) { return mesh.Triangles; }
};

}
}

#endif
