#ifndef DTPOINT_HH
#define DTPOINT_HH

#define EPSILON 1.e-12

#include <cmath>
#include <ostream>

class DTPoint
{
public:
  DTPoint(double xx,double yy) : x(xx), y(yy) {}

  double Distance(const DTPoint& p) const { return sqrt((x-p.x)*(x-p.x) + (y-p.y)*(y-p.y)); }
  
  const double x;
  const double y;

  friend bool operator==(const DTPoint& p1,const DTPoint& p2)
  {
    return (fabs(p1.x - p2.x) < EPSILON && fabs(p1.y - p2.y) < EPSILON);
  }

  friend bool operator!=(const DTPoint& p1,const DTPoint& p2)
  {
    return !(p1 == p2);
  }

  friend bool operator<(const DTPoint& p1,const DTPoint& p2)
  {
    return (p1.x != p2.x ? p1.x < p2.x : p1.y < p2.y);
  }

  friend bool operator>=(const DTPoint& p1,const DTPoint& p2)
  {
    return !(p1<p2);
  }
  
  friend bool operator>(const DTPoint& p1,const DTPoint& p2)
  {
    return (p1.x != p2.x ? p1.x > p2.x : p1.y > p2.y);
  }

  friend bool operator<=(const DTPoint& p1,const DTPoint& p2)
  {
    return !(p1>p2);
  }

  friend DTPoint operator*(double d,const DTPoint& p)
  {
    return DTPoint(p.x*d,p.y*d);
  }

  friend DTPoint operator*(const DTPoint& p,double d)
  {
    return operator*(d,p);
  }

  friend DTPoint operator/(const DTPoint& p,double d)
  {
    return DTPoint(p.x/d,p.y/d);
  }

  friend DTPoint operator+(const DTPoint& p1,const DTPoint& p2)
  {
    return DTPoint(p1.x + p2.x,p1.y + p2.y);
  }

  friend DTPoint operator-(const DTPoint& p1,const DTPoint& p2)
  {
    return DTPoint(p1.x - p2.x,p1.y - p2.y);
  }

  friend std::ostream& operator<<(std::ostream& s,const DTPoint& p)
  {
    s<<"("<< p.x<<","<<p.y<<")";
    return s;
  }
};


#endif /* DTPOINT_HH */
