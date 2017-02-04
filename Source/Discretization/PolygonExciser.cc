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

#include "Discretization/PolygonExciser.hh"

#include "Discretization/EdgeInserter.hh"
#include "Mesh/Edge.hh"
#include "Mesh/Vertex.hh"
#include "Shape/LineSegmentUtilities.hh"

#include <cassert>

namespace Delaunay
{
namespace Discretization
{

std::set<const Mesh::Edge*> PolygonExciser::ExcisePolygon(
  const Delaunay::Shape::Polygon& polygon, Delaunay::Mesh::Mesh& mesh)
{
  EdgeInserter edgeInserter;

  // keep track of edges and their orientation w.r.t. their counterclockwise
  // ordering in the polygon
  std::vector<std::pair<const Mesh::Edge*,int> > newEdges;

  for (unsigned i = 0; i < polygon.GetPoints().size(); ++i)
  {
    unsigned ipp = (i+1)%polygon.GetPoints().size();
    Shape::LineSegment l(polygon.GetPoints()[i], polygon.GetPoints()[ipp]);
    std::set<const Mesh::Edge*> edges(
      std::move(edgeInserter.InsertEdge(l, mesh)));
    for (auto& edge : edges)
    {
      const_cast<Mesh::Edge*>(edge)->boundary = true;
      bool isCCW =
	Shape::Dot(edge->B() - edge->A(),
		   polygon.GetPoints()[ipp] - polygon.GetPoints()[i]) > 0.;

      newEdges.push_back(std::make_pair(edge, isCCW ? 1 : -1));
    }
  }

  // Now that everything is added, we can find and remove the triangles and
  // edges in the polygon.
  std::set<const Mesh::Triangle*> trianglesToRemove;
  std::set<const Mesh::Edge*> edgesToRemove;
  for (auto& edge : newEdges)
  {
    // there are two triangles associated with this edge. We are looking to
    // eliminate the one whose third point is counterclockwise to the points
    // that comprise the edge
    const Mesh::Triangle* triangle = *edge.first->triangles.begin();
    const Mesh::Vertex* v = &triangle->A();
    if (v == &edge.first->A() || v == &edge.first->B())
      v = &triangle->B();
    if (v == &edge.first->A() || v == &edge.first->B())
      v = &triangle->C();

    if (Orientation(edge.first->A(), edge.first->B(), *v) != edge.second)
      triangle = *(++(edge.first->triangles.begin()));

    std::vector<const Mesh::Triangle*> trianglesToWalk;
    if (trianglesToRemove.find(triangle) == trianglesToRemove.end())
      trianglesToWalk.push_back(triangle);

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
  }

  for (auto triangle : trianglesToRemove)
    this->GetTriangles(mesh).erase(*triangle);

  for (auto edge : edgesToRemove)
    this->GetEdges(mesh).erase(*edge);

  std::set<const Mesh::Edge*> innerEdges;
  for (auto edge : newEdges)
    innerEdges.insert(edge.first);

  return innerEdges;
}

}
}
