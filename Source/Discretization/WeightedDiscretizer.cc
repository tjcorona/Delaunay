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

#include <numeric>

#include "Discretization/WeightedDiscretizer.hh"

#include "Shape/TriangleUtilities.hh"

#include "Discretization/DelaunayDiscretizer.hh"

namespace Delaunay
{
namespace Discretization
{

WeightedDiscretizer::WeightedDiscretizer() :
  W([](const Shape::Point&) { return 1.; })
{
}

WeightedDiscretizer::WeightedDiscretizer(const WeightFunction& w) : W(w)
{
}

namespace
{
Shape::Point Barycenter(const Shape::Point& p,
			const Shape::Triangle& t,
			const WeightFunction& w)
{
  double wa = w(t.AB.A);
  double wb = w(t.AB.B);
  double wc = w(t.AC.B);

  return (t.AB.A*wa + t.AB.B*wb + t.AC.B*wc)/(wa + wb + wc);
}
}

void WeightedDiscretizer::Mesh(const Delaunay::Shape::Polygon& polygon,
			       Delaunay::Mesh::Mesh& mesh)
{
  DelaunayDiscretizer delaunay;
  delaunay.Mesh(polygon, mesh);

  double area = std::accumulate(mesh.GetTriangles().begin(),
				mesh.GetTriangles().end(), 0.,
				[&](double a, const Shape::Triangle& b)
				{ return a + Shape::Area(b); });
}

}
}
