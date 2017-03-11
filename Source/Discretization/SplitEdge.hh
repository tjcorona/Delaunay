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

#ifndef DELAUNAY_DISCRETIZATION_SPLITEDGE_HH
#define DELAUNAY_DISCRETIZATION_SPLITEDGE_HH

#include "Mesh/Edge.hh"
#include "Mesh/Mesher.hh"
#include "Mesh/TriangleSet.hh"

namespace Delaunay
{
namespace Discretization
{

class SplitEdge : public Mesh::Mesher
{
public:
  SplitEdge() {}

  std::pair<const Mesh::Edge*, const Mesh::Edge*>
  operator()(const Mesh::Edge&, Delaunay::Mesh::Mesh&) const;

  std::pair<const Mesh::Edge*, const Mesh::Edge*>
  operator()(const Mesh::Edge&, double, Delaunay::Mesh::Mesh&) const;
};

}
}

#endif
