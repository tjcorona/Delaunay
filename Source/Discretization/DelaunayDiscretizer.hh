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

#ifndef DELAUNAY_DISCRETIZATION_DELAUNAYDISCRETIZER_HH
#define DELAUNAY_DISCRETIZATION_DELAUNAYDISCRETIZER_HH

#include <deque>

#include "Shape/Point.hh"
#include "Mesh/Mesher.hh"
#include "Mesh/TriangleSet.hh"

namespace Delaunay
{
namespace Discretization
{

class DelaunayDiscretizer : public Mesh::Mesher
{
public:
  typedef Shape::Point Point;
  typedef Shape::Triangle Triangle;

  DelaunayDiscretizer() {}

  void Mesh(const Delaunay::Shape::Polygon&, Delaunay::Mesh::Mesh&);

  void AddPerimeterPoint(const Point&, Delaunay::Mesh::Mesh&);
  void AddInteriorPoint(const Point&, Delaunay::Mesh::Mesh&);

  bool TestDelaunayCondition(Mesh::TriangleSet& illegalTriangles,
			     const Delaunay::Mesh::Mesh&) const;

private:
  void ConstructInitialMeshFromBoundaries(Delaunay::Mesh::Mesh&);
  const Mesh::Triangle* FindContainingTriangle(const Point& p,
					 Delaunay::Mesh::Mesh&) const;
  void SplitTriangle(const Mesh::Triangle*, const Mesh::Vertex*,
		     Delaunay::Mesh::Mesh&);
  void ExtendMesh(const Mesh::Vertex*, Delaunay::Mesh::Mesh&);
  void LegalizeEdges(const Mesh::Vertex*, std::set<const Mesh::Edge*>&,
		     Delaunay::Mesh::Mesh&);
};

}
}

#endif
