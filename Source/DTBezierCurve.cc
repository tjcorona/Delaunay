#include "DTBezierCurve.hh"

#include "DTPoint.hh"

const DTPoint DTBezierCurve::operator() (double t) const
{
  std::vector<DTPoint> v1 = points;
  if (Closed())
    v1.push_back(DTPoint(points.front()));
  std::vector<DTPoint> v2;

  return RecursiveBezier(t,v1,v2);
}

const DTPoint DTBezierCurve::RecursiveBezier(double t,std::vector<DTPoint>& v1,std::vector<DTPoint>& v2) const
{
  v2.clear();
  for (size_t i=0;i<v1.size()-1;i++)
  {
    v2.push_back(DTPoint((1.-t)*v1[i] + t*v1[i+1]));
  }
  if (v2.size()>1)
    return RecursiveBezier(t,v2,v1);
  else
    return v2[0];
}

std::vector<DTPoint> DTBezierCurve::C1Continuity() const
{
  
}
