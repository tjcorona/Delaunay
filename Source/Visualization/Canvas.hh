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

#ifndef DELAUNAY_VISUALIZATION_CANVAS_HH
#define DELAUNAY_VISUALIZATION_CANVAS_HH

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

class Canvas
{
public:
  virtual ~Canvas() {}

  virtual void Draw(const Misc::Function&,double*,const Color&) = 0;

  virtual void Draw(const Shape::Point&,const Color&) = 0;
  virtual void Draw(const Shape::LineSegment&,const Color&) = 0;
  virtual void Draw(const Shape::Triangle&,const Color&,const Color&) = 0;
  virtual void Draw(const Shape::Polygon&,const Color&,const Color&) = 0;
  virtual void Draw(const Shape::Circle&,const Color&,const Color&) = 0;
  virtual void Draw(const Shape::ParametricCurve&,const Color&) = 0;
  virtual void Update() {}
};

}
}

#endif
