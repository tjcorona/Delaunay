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

#ifndef DELAUNAY_VISUALIZATION_VTKCANVAS_HH
#define DELAUNAY_VISUALIZATION_VTKCANVAS_HH

#include "Canvas.hh"

#include "Color.hh"

namespace Delaunay
{

namespace Misc
{
class Function;
}

namespace Shape
{
class Circle;
class LineSegment;
class ParametricCurve;
class Point;
class Polygon;
class Triangle;
}

namespace Visualization
{

class Color;

class VTKCanvas : public Canvas
{
public:
  VTKCanvas(double dim_x_min,
	    double dim_x_max,
	    double dim_y_min,
	    double dim_y_max);
  virtual ~VTKCanvas();

  void Draw(const Misc::Function&,double* =nullptr,const Color& color=Blue);
  void Draw(const Shape::Point&,const Color& color=Green);
  void Draw(const Shape::LineSegment&,const Color& color=Orange);
  void Draw(const Shape::Triangle&,const Color& lineColor=Green,
	    const Color& fillColor=Clear);
  void Draw(const Shape::Polygon&,const Color& lineColor=Red,
	    const Color& fillColor=Clear);
  void Draw(const Shape::Circle&,const Color& lineColor=Black,
	    const Color& fillColor=Clear);
  void Draw(const Shape::ParametricCurve&,const Color& color=Cyan);
  void Update();
  void SetTimeDelay(double d);

private:
  class Internals;
  Internals* internals;
};

}
}

#endif
