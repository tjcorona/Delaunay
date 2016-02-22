#ifndef DELAUNAY_SHAPE_POINT_HH
#define DELAUNAY_SHAPE_POINT_HH

#define EPSILON 1.e-12
// #define GRIDRES 8

#include <cmath>
#include <ostream>

namespace Delaunay
{
namespace Shape
{

class Point
{
public:
  static double ToGrid(double x)
  {
#ifdef GRIDRES
    return std::floor(x * std::pow(10,GRIDRES) + 0.5)/std::pow(10,GRIDRES);
#else
    return x;
#endif
  }

  Point(double xx,double yy) : x(ToGrid(xx)), y(ToGrid(yy)) {}

  double Distance(const Point& p) const { return sqrt((x-p.x)*(x-p.x) + (y-p.y)*(y-p.y)); }

  const double x;
  const double y;

  friend bool operator==(const Point& p1,const Point& p2)
  {
    return (fabs(p1.x - p2.x) < EPSILON && fabs(p1.y - p2.y) < EPSILON);
  }

  friend bool operator!=(const Point& p1,const Point& p2)
  {
    return !(p1 == p2);
  }

  friend bool operator<(const Point& p1,const Point& p2)
  {
    return (p1.x != p2.x ? p1.x < p2.x : p1.y < p2.y);
  }

  friend bool operator>=(const Point& p1,const Point& p2)
  {
    return !(p1<p2);
  }

  friend bool operator>(const Point& p1,const Point& p2)
  {
    return (p1.x != p2.x ? p1.x > p2.x : p1.y > p2.y);
  }

  friend bool operator<=(const Point& p1,const Point& p2)
  {
    return !(p1>p2);
  }

  friend Point operator*(double d,const Point& p)
  {
    return Point(p.x*d,p.y*d);
  }

  friend Point operator*(const Point& p,double d)
  {
    return operator*(d,p);
  }

  friend Point operator/(const Point& p,double d)
  {
    return Point(p.x/d,p.y/d);
  }

  friend Point operator+(const Point& p1,const Point& p2)
  {
    return Point(p1.x + p2.x,p1.y + p2.y);
  }

  friend Point operator-(const Point& p1,const Point& p2)
  {
    return Point(p1.x - p2.x,p1.y - p2.y);
  }

  friend double Dot(const Point& p1,const Point& p2)
  {
    return p1.x*p2.x + p1.y*p2.y;
  }

  friend std::ostream& operator<<(std::ostream& s,const Point& p)
  {
    s<<"("<< p.x<<","<<p.y<<")";
    return s;
  }
};

}
}

#endif
