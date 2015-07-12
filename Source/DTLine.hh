#ifndef DTLINE_HH
#define DTLINE_HH

#include "DTPoint.hh"

class DTLine
{
public:
  DTLine(const DTPoint& p1,const DTPoint& yy) : x(xx), y(yy) {}

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

  friend std::ostream& operator<<(std::ostream& s,const DTPoint& p)
  {
    s<<"("<< p.x<<","<<p.y<<")";
    return s;
  }
};


#endif /* DTLINE_HH */
