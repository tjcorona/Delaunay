#ifndef DTBEZIERCURVE_HH
#define DTBEZIERCURVE_HH

#include <vector>

#include "DTParametricCurve.hh"
#include "DTPoint.hh"

class DTPoint;

class DTBezierCurve : public DTParametricCurve
{
public:
  const DTPoint operator() (double) const;

  void AddPoint(const DTPoint& p) { points.push_back(DTPoint(p)); }

protected:
  const DTPoint RecursiveBezier(double,std::vector<DTPoint>&,std::vector<DTPoint>&) const;
  std::vector<DTPoint> C1Continuity() const;
  
  std::vector<DTPoint> points;
};


#endif /* DTBEZIERCURVE_HH */
