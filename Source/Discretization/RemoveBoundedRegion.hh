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

#ifndef DELAUNAY_DISCRETIZATION_REMOVEBOUNDEDREGION_HH
#define DELAUNAY_DISCRETIZATION_REMOVEBOUNDEDREGION_HH

#include "Discretization/Export.hh"

#include "Shape/Polygon.hh"
#include "Mesh/Mesher.hh"

namespace Delaunay
{
namespace Discretization
{

class DELAUNAYDISCRETIZATION_EXPORT RemoveBoundedRegion : public Mesh::Mesher
{
public:
  RemoveBoundedRegion() {}

  void operator()(const Mesh::Edge&, bool, Mesh::Mesh&);
  void operator()(const Mesh::Triangle&, Mesh::Mesh&);
};

}
}

#endif
