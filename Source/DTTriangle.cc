#include "DTTriangle.hh"

DTTriangle::DTTriangle(const DTPoint& AA,
		       const DTPoint& BB,
		       const DTPoint& CC) : A(AA), B(BB), C(CC)
{
  circumcenter = ComputeCircumcenter();
  circumradius = circumcenter.Distance(A);
}

DTPoint DTTriangle::ComputeCircumcenter() const
{
  double D = 2.*(A.x*(B.y - C.y) + B.x*(C.y - A.y) + C.x*(A.y - B.y));

  DTPoint U(((A.x*A.x + A.y*A.y)*(B.y-C.y) +
	     (B.x*B.x + B.y*B.y)*(C.y-A.y) +
	     (C.x*C.x + C.y*C.y)*(A.y-B.y))/D,
	    ((A.x*A.x + A.y*A.y)*(C.x-B.x) +
	     (B.x*B.x + B.y*B.y)*(A.x-C.x) +
	     (C.x*C.x + C.y*C.y)*(B.x-A.x))/D);
  return U;
}
