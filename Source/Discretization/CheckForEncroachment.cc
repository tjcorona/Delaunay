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

#include "Discretization/CheckForEncroachment.hh"

#include "Shape/Circle.hh"
#include "Shape/CircleUtilities.hh"
#include "Shape/LineSegmentUtilities.hh"

namespace Delaunay
{
namespace Discretization
{

bool CheckForEncroachment::operator()(
  const Shape::Point& p, const Delaunay::Mesh::Edge& edge) const
{
  Shape::Circle c((edge.A() + edge.B())/2., Shape::Length(edge)/2.);
  return Shape::Contains(c,p);
}

std::set<const Mesh::Edge*> CheckForEncroachment::operator()(
  const Shape::Point& p, Delaunay::Mesh::Mesh& mesh) const
{
  std::set<const Mesh::Edge*> encroached;

  for (auto& edge : mesh.GetEdges())
    if (this->operator()(p, edge))
      encroached.insert(&edge);

  return encroached;
}

}
}
