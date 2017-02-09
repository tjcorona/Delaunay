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

#ifndef DELAUNAY_DISCRETIZATION_EXTENDMESH_HH
#define DELAUNAY_DISCRETIZATION_EXTENDMESH_HH

#include "Shape/Point.hh"
#include "Mesh/Mesher.hh"
#include "Mesh/TriangleSet.hh"

namespace Delaunay
{
namespace Discretization
{

class ExtendMesh : public Mesh::Mesher
{
public:
  typedef Shape::Point Point;
  typedef Shape::Triangle Triangle;

  ExtendMesh() {}

  void operator()(const Point&, Delaunay::Mesh::Mesh&);

private:
  void Extend(const Mesh::Vertex*, Delaunay::Mesh::Mesh&);
};

}
}

#endif
