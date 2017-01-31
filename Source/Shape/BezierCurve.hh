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

#ifndef DELAUNAY_SHAPE_BEZIERCURVE_HH
#define DELAUNAY_SHAPE_BEZIERCURVE_HH

#include <vector>

#include "ParametricCurve.hh"
#include "Point.hh"

namespace Delaunay
{
namespace Shape
{

class BezierCurve : public ParametricCurve
{
public:
  BezierCurve(const PointVector&,bool closed=false);

  const Point operator() (double) const;

protected:
  const Point RecursiveBezier(double,std::vector<Point>&,
			      std::vector<Point>&) const;

  PointVector Points;
};

}
}

#endif
