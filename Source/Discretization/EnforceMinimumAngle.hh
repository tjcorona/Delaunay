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

#ifndef DELAUNAY_DISCRETIZATION_ENFORCEMINIMUMANGLE_HH
#define DELAUNAY_DISCRETIZATION_ENFORCEMINIMUMANGLE_HH

#include "Mesh/Mesher.hh"

namespace Delaunay
{
namespace Discretization
{

// WIP
class EnforceMinimumAngle : public Mesh::Mesher
{
public:
  EnforceMinimumAngle() {}

  void operator()(double angle, Delaunay::Mesh::Mesh&) const;

  bool IsEncroached(const Mesh::Edge&) const;

protected:
  double MinimumAngle(const Mesh::Triangle&) const;
  std::set<const Mesh::Edge*> Encroaches(
    const Shape::Point& p, const Delaunay::Mesh::Mesh& mesh) const;
  void RecursivelySplitEdge(const Mesh::Edge&, Delaunay::Mesh::Mesh&) const;
};

}
}

#endif
