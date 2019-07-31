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

#include "Shape/Export.hh"

#include "Shape/ParametricCurve.hh"
#include "Shape/Point.hh"

namespace Delaunay
{
namespace Shape
{

class DELAUNAYSHAPE_EXPORT BezierCurve : public ParametricCurve
{
public:
  BezierCurve(const PointList&,bool closed=false);

  const Point operator() (double) const;

protected:
  const Point RecursiveBezier(double,std::vector<Point>&,
			      std::vector<Point>&) const;

  PointList Points;
};

}
}

#endif
