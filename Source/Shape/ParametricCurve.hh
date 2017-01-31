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

#ifndef DELAUNAY_SHAPE_PARAMETRICCURVE_HH
#define DELAUNAY_SHAPE_PARAMETRICCURVE_HH

namespace Delaunay
{
namespace Shape
{

class Point;

class ParametricCurve
{
public:
  ParametricCurve(bool c) : Closed(c) {}
  virtual ~ParametricCurve() {}

  virtual const Point operator() (double) const = 0;

  const bool Closed;
};

}
}

#endif
