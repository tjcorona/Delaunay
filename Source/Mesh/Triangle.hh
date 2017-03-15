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

#ifndef DELAUNAY_MESH_TRIANGLE_HH
#define DELAUNAY_MESH_TRIANGLE_HH

#include <set>

#include "Mesh/Export.hh"

#include "Mesh/Vertex.hh"
#include "Mesh/Edge.hh"

#include "Shape/Triangle.hh"

namespace Delaunay
{
namespace Mesh
{

class DELAUNAYMESH_EXPORT Triangle : public Shape::Triangle
{
public:
  Triangle(const Edge& ab,const Edge& bc,const Edge& ac);

  ~Triangle();

  const Edge& AB() const;
  const Edge& BC() const;
  const Edge& AC() const;
  const Vertex& A() const;
  const Vertex& B() const;
  const Vertex& C() const;
};

}
}

#endif
