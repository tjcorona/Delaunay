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

#ifndef DELAUNAY_DISCRETIZATION_EXCISEPOLYGON_HH
#define DELAUNAY_DISCRETIZATION_EXCISEPOLYGON_HH

#include "Shape/Point.hh"
#include "Mesh/Mesher.hh"
#include "Mesh/TriangleSet.hh"

namespace Delaunay
{
namespace Discretization
{

class ExcisePolygon : public Mesh::Mesher
{
public:
  typedef Shape::Point Point;
  typedef Shape::Triangle Triangle;

  ExcisePolygon() {}

  std::set<const Mesh::Edge*> operator()(const Delaunay::Shape::Polygon&,
					 Delaunay::Mesh::Mesh&);
};

}
}

#endif
