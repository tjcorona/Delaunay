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

#ifndef DELAUNAY_SHAPE_POLYGONUTILITIES_HH
#define DELAUNAY_SHAPE_POLYGONUTILITIES_HH

#include <array>

namespace Delaunay
{
namespace Shape
{
class Polygon;

std::array<double, 4> Bounds(const Polygon&);

  // +1: counterclockwise
  // -1: clockwise
int Orientation(const Polygon&);
}
}

#endif
