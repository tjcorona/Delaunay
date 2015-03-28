#ifndef DTPOINT_HH
#define DTPOINT_HH

#include <cmath>

class DTPoint
{
public:
  DTPoint() : x(0), y(0) {}
  DTPoint(double xx,double yy) : x(xx), y(yy) {}

  double Distance(const DTPoint& p) const { return sqrt((x-p.x)*(x-p.x) + (y-p.y)*(y-p.y)); }
  
  double x;
  double y;
};

#endif /* DTPOINT_HH */
