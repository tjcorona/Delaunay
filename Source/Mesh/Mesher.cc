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

#include "Mesh/Mesher.hh"

#include "Mesh/Mesh.hh"

namespace Delaunay
{
namespace Mesh
{

std::pair<Mesh::VertexSet::const_iterator,bool>
Mesher::InsertVertex(const Shape::Point& p, Mesh& mesh)
{
  return this->GetVertices(mesh).emplace(p);
}

std::pair<Mesh::EdgeSet::const_iterator,bool>
Mesher::InsertEdge(const Vertex& a, const Vertex& b, Mesh& mesh)
{
  return this->GetEdges(mesh).emplace(a,b);
}

std::pair<Mesh::TriangleSet::const_iterator,bool>
Mesher::InsertTriangle(const Edge& ab, const Edge& bc, const Edge& ac,
                       Mesh& mesh)
{
  return this->GetTriangles(mesh).emplace(ab,bc,ac);
}

}
}
