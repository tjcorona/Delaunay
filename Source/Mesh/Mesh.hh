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

#ifndef DELAUNAY_MESH_MESH_HH
#define DELAUNAY_MESH_MESH_HH

#include <set>

#include "Mesh/Export.hh"

#include "Mesh/Vertex.hh"
#include "Mesh/Edge.hh"
#include "Mesh/Polygon.hh"
#include "Mesh/Triangle.hh"

namespace Delaunay
{
namespace Mesh
{
class Mesher;

class DELAUNAYMESH_EXPORT Mesh
{
public:
  typedef Polygon            ExteriorBoundary;
  typedef std::set<Polygon>  InteriorBoundarySet;
  typedef std::set<Vertex>   VertexSet;
  typedef std::set<Edge>     EdgeSet;
  typedef std::set<Triangle> TriangleSet;

  friend class Mesher;

  Mesh() : Perimeter(VertexList()) {}
  virtual ~Mesh() {}

  const ExteriorBoundary& GetPerimeter() const { return this->Perimeter; }
  const InteriorBoundarySet&
            GetInteriorBoundaries() const { return this->InteriorBoundaries; }
  const VertexSet&   GetVertices()  const { return this->Vertices;  }
  const EdgeSet&     GetEdges()     const { return this->Edges;     }
  const TriangleSet& GetTriangles() const { return this->Triangles; }

  virtual const Triangle* FindContainingTriangle(const Shape::Point& p) const;
  virtual const Triangle* FindContainingTriangle(
    const Shape::Point&, const Triangle*) const;

protected:
  ExteriorBoundary    Perimeter;
  InteriorBoundarySet InteriorBoundaries;
  VertexSet           Vertices;
  EdgeSet             Edges;
  TriangleSet         Triangles;
};

}
}

#endif
