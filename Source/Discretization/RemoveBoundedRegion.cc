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

#include "RemoveBoundedRegion.hh"

#include <set>
#include <vector>

namespace Delaunay
{
namespace Discretization
{

void RemoveBoundedRegion::operator()(const Mesh::Edge& edge, bool isCCW,
				     Mesh::Mesh& mesh)
{
  // there are two triangles associated with this edge. We are looking to
  // eliminate the one whose third point is oriented <isCCW>.
  const Mesh::Triangle* triangle = *edge.triangles.begin();
  const Mesh::Vertex* v = &triangle->A();
  if (v == &edge.A() || v == &edge.B())
    v = &triangle->B();
  if (v == &edge.A() || v == &edge.B())
    v = &triangle->C();

  if (Orientation(edge.A(), edge.B(), *v) != (isCCW ? 1 : -1))
    triangle = *(++(edge.triangles.begin()));

  return this->operator()(*triangle, mesh);
}

void RemoveBoundedRegion::operator()(const Mesh::Triangle& triangle,
                                     Mesh::Mesh& mesh)
{
  std::set<const Mesh::Triangle*> trianglesToRemove;
  std::set<const Mesh::Edge*> edgesToRemove;

  std::vector<const Mesh::Triangle*> trianglesToWalk;
  trianglesToWalk.push_back(&triangle);

  while (!trianglesToWalk.empty())
  {
    const Mesh::Triangle* tri = trianglesToWalk.back();
    trianglesToWalk.pop_back();
    if (trianglesToRemove.insert(tri).second == true)
    {
      const Mesh::Edge* edges[3] = {&tri->AB(), &tri->BC(), &tri->AC()};
      for (unsigned i=0;i<3;i++)
      {
        if (edges[i]->boundary)
          continue;
        else
          edgesToRemove.insert(edges[i]);
        for (auto adjacentTri : edges[i]->triangles)
          if (adjacentTri != tri)
            trianglesToWalk.push_back(adjacentTri);
      }
    }
  }

  for (auto triangle : trianglesToRemove)
    this->GetTriangles(mesh).erase(*triangle);

  for (auto edge : edgesToRemove)
    this->GetEdges(mesh).erase(*edge);
}

}
}
