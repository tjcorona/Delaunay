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

#ifndef DELAUNAY_DISCRETIZATION_LEGALIZEEDGES_HH
#define DELAUNAY_DISCRETIZATION_LEGALIZEEDGES_HH

#include "Discretization/Export.hh"

#include "Shape/Point.hh"
#include "Mesh/Mesher.hh"
#include "Mesh/TriangleSet.hh"

namespace Delaunay
{
namespace Discretization
{

class DELAUNAYDISCRETIZATION_EXPORT LegalizeEdges : public Mesh::Mesher
{
public:
  LegalizeEdges() {}
  void operator()(const Mesh::Vertex*, std::set<const Mesh::Edge*>&,
		  Delaunay::Mesh::Mesh&) const;
};

}
}

#endif
