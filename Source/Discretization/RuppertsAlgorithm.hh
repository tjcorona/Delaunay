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

#ifndef DELAUNAY_DISCRETIZATION_RUPPERTSALGORITHM_HH
#define DELAUNAY_DISCRETIZATION_RUPPERTSALGORITHM_HH

#include "Shape/Polygon.hh"
#include "Mesh/Mesher.hh"

#include <forward_list>
#include <functional>

namespace Delaunay
{
namespace Discretization
{

class RuppertsAlgorithm : public Mesh::Mesher
{
public:
  typedef std::forward_list<
      std::reference_wrapper<const Mesh::Vertex> > VertexList;

  RuppertsAlgorithm() {}

  void operator()(const Delaunay::Shape::Polygon&, Delaunay::Mesh::Mesh&);

protected:
  double MinimumAngle(const Mesh::Triangle&) const;

  bool IsEncroached(const VertexList::const_iterator&,
		    const VertexList::const_iterator&,
		    const Mesh::Mesh&) const;

  std::vector<VertexList::const_iterator> Encroaches(const Shape::Point&,
						     const VertexList&) const;

  void RecursivelySplitEdge(const VertexList::const_iterator&,
			    const VertexList::const_iterator&, VertexList&,
			    Delaunay::Mesh::Mesh&) const;

  const Mesh::Triangle* FindContainingTriangle(const Shape::Point&,
					       const Delaunay::Mesh::Mesh&) const;
};

}
}

#endif
