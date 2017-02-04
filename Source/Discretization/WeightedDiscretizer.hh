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

#ifndef DELAUNAY_DISCRETIZATION_WEIGHTEDDISCRETIZER_HH
#define DELAUNAY_DISCRETIZATION_WEIGHTEDDISCRETIZER_HH

#include <functional>

#include "Shape/Point.hh"
#include "Shape/Polygon.hh"
#include "Mesh/Mesh.hh"
#include "Mesh/Mesher.hh"

namespace Delaunay
{
namespace Discretization
{

typedef std::function<double(const Shape::Point&)> WeightFunction;

class WeightedDiscretizer : public Mesh::Mesher
{
public:
  WeightedDiscretizer();
  WeightedDiscretizer(const WeightFunction&);

  void SetWeightFunction(const WeightFunction& w) { this->W = w; }

  void Mesh(const Delaunay::Shape::Polygon&, Delaunay::Mesh::Mesh&);

protected:
  WeightFunction W;
};

}
}

#endif
