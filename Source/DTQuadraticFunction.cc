#include "DTQuadraticFunction.hh"

#include "DTPoint.hh"

DTQuadraticFunction::DTQuadraticFunction()
{
  coefficients[0] = 1.;
  coefficients[1] = 0.;
  coefficients[2] = 0.;
}

double DTQuadraticFunction::operator() (double x) const
{
  return coefficients[0]*x*x + coefficients[1]*x + coefficients[2];
}

void DTQuadraticFunction::SetCoefficients(double a, double b, double c)
{
  coefficients[0] = a;
  coefficients[1] = b;
  coefficients[2] = c;
}

void DTQuadraticFunction::SetCoefficients(double* coeff)
{
  SetCoefficients(coeff[0],coeff[1],coeff[2]);
}
