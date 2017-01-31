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

#ifndef DELAUNAY_SHAPE_CIRCLE_HH
#define DELAUNAY_SHAPE_CIRCLE_HH

#include "Point.hh"

namespace Delaunay
{
namespace Shape
{

class Circle
{
public:
  Circle(const Point& c, double r) : Center(c), Radius(r) {}

  const Point& Center;
  const double Radius;
};

}
}

#endif
