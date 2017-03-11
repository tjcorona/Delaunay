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

#include "Discretization/AddInteriorPoint.hh"

#include "Discretization/SplitTriangle.hh"

#include <cassert>

namespace Delaunay
{
namespace Discretization
{

const Mesh::Vertex* AddInteriorPoint::operator()(const Point& p,
						 Delaunay::Mesh::Mesh& mesh)
{
  assert(this->GetTriangles(mesh).size() != 0);

  const Mesh::Triangle* containingTriangle = mesh.FindContainingTriangle(p);

  static const SplitTriangle splitTriangle;
  return (containingTriangle ? splitTriangle(*containingTriangle, p, mesh) :
          nullptr);
}

}
}
