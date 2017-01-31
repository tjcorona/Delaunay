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
