#ifndef DTQUADRATICFUNCTION_HH
#define DTQUADRATICFUNCTION_HH

#include "DTFunction.hh"

class DTPoint;

class DTQuadraticFunction : public DTFunction
{
public:
  DTQuadraticFunction();
  double operator() (double x) const;

  void SetCoefficients(double aa, double bb, double cc);
  void SetCoefficients(double* coeff);

protected:
  double coefficients[3];
};


#endif /* DTFUNCTION_HH */
