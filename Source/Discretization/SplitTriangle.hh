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

#ifndef DELAUNAY_DISCRETIZATION_SPLITTRIANGLE_HH
#define DELAUNAY_DISCRETIZATION_SPLITTRIANGLE_HH

#include "Mesh/Mesher.hh"

namespace Delaunay
{
namespace Discretization
{

class SplitTriangle : public Mesh::Mesher
{
public:
  SplitTriangle() {}

  const Mesh::Vertex* operator()(const Mesh::Triangle&,
                                 Delaunay::Mesh::Mesh&) const;

  const Mesh::Vertex* operator()(const Mesh::Triangle&,
                                 double barycentricCoords[3],
                                 Delaunay::Mesh::Mesh&) const;

  const Mesh::Vertex* operator()(const Mesh::Triangle&,
                                 const Shape::Point&,
                                 Delaunay::Mesh::Mesh&) const;

};

}
}

#endif
