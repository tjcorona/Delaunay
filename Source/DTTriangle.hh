#ifndef DTTRIANGLE_HH
#define DTTRIANGLE_HH

#include "DTPoint.hh"

class DTTriangle
{
public:
  DTTriangle(const DTPoint& AA,const DTPoint& BB,const DTPoint& CC);

  const DTPoint& A;
  const DTPoint& B;
  const DTPoint& C;
  DTPoint circumcenter;
  double circumradius;

private:
  DTPoint ComputeCircumcenter() const;
};

#endif /* DTTRIANGLE_HH */
