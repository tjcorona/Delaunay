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

#include "Discretization/TestDelaunayCondition.hh"

#include "Shape/CircleUtilities.hh"
#include "Shape/PointUtilities.hh"
#include "Shape/TriangleUtilities.hh"

#include <cassert>
#include <exception>
#include <limits>

namespace Delaunay
{
namespace Discretization
{

bool TestDelaunayCondition::operator()(Mesh::TriangleSet& illegalTriangles,
				       const Delaunay::Mesh::Mesh& mesh) const
{
  for (auto edge=mesh.GetEdges().begin();edge!=mesh.GetEdges().end();++edge)
  {
    if ((*edge).triangles.size() < 2)
      continue;

    const Mesh::Triangle* t1 = *((*edge).triangles.begin());
    const Mesh::Triangle* t2 = *(++((*edge).triangles.begin()));

    const Mesh::Vertex* I = &((*edge).A());
    const Mesh::Vertex* J = &((*edge).B());
    const Mesh::Vertex* K = &(t1->A());
    if (K == I || K == J)
      K = &(t1->B());
    if (K == I || K == J)
      K = &(t1->C());
    const Mesh::Vertex* L = &(t2->A());
    if (L == I || L == J)
      L = &(t2->B());
    if (L == I || L == J)
      L = &(t2->C());

    bool isLegal = true;

    const Point* C = &(t1->circumcircle.Center);

    double d2 = (C->x-L->x)*(C->x-L->x) + (C->y-L->y)*(C->y-L->y);
    if (d2 + EPSILON < t1->circumcircle.Radius*t1->circumcircle.Radius)
    {
      isLegal = false;
    }

    C = &(t2->circumcircle.Center);
    d2 = (C->x-K->x)*(C->x-K->x) + (C->y-K->y)*(C->y-K->y);
    if (d2  + EPSILON < t2->circumcircle.Radius*t2->circumcircle.Radius)
    {
      isLegal = false;
    }

    if (!isLegal)
    {
      illegalTriangles.insert(t1);
      illegalTriangles.insert(t2);
    }
  }
  return illegalTriangles.empty();
}

}
}
