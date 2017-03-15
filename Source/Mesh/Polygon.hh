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

#ifndef DELAUNAY_MESH_POLYGON_HH
#define DELAUNAY_MESH_POLYGON_HH

#include <vector>

#include "Mesh/Export.hh"

#include "Mesh/Vertex.hh"
#include "Shape/Polygon.hh"

namespace Delaunay
{
namespace Mesh
{

class DELAUNAYMESH_EXPORT Polygon : public Shape::Polygon
{
public:
  Polygon(const Shape::PointVector&);
  ~Polygon() {}

  unsigned size() const { return this->Points.size(); }
  const Vertex& operator[](unsigned) const;

private:

};

}
}

#endif
