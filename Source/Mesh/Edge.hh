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

#ifndef DELAUNAY_MESH_EDGE_HH
#define DELAUNAY_MESH_EDGE_HH

#include <set>

#include "Shape/LineSegment.hh"
#include "Mesh/Vertex.hh"

namespace Delaunay
{
namespace Mesh
{

class Triangle;
typedef std::set<const Triangle*> TriangleSet;

class Edge : public Shape::LineSegment
{
public:
  Edge(const Vertex& a, const Vertex& b, bool bndry = false) : LineSegment(a,b),
							       boundary(bndry)
  {
    this->A().edges.insert(this);
    this->B().edges.insert(this);
  }

  ~Edge()
  {
    this->A().edges.erase(this);
    this->B().edges.erase(this);
  }

  const Vertex& A() const
  {
    return static_cast<const Vertex&>(this->Shape::LineSegment::A);
  }
  const Vertex& B() const
  {
    return static_cast<const Vertex&>(this->Shape::LineSegment::B);
  }

  bool boundary;

  mutable TriangleSet triangles;
};

}
}

#endif
