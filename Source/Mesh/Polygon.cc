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

#include "Mesh/Polygon.hh"

#include "Shape/PointUtilities.hh"
#include "Shape/LineSegment.hh"
#include "Shape/LineSegmentUtilities.hh"

#include <algorithm>
#include <cassert>

namespace Delaunay
{
namespace Mesh
{

Polygon::Polygon()
  : Shape::Polygon(Shape::PointList())
  , vertices(Points)
{
}

Polygon::Polygon(const VertexList& list)
  : Polygon()
{
  SetVertices(list);
}

void Polygon::SetVertices(const VertexList& list)
{
  vertices.assign(list.begin(), list.end());
}

}
}
