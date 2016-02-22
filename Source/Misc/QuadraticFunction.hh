#ifndef DELAUNAY_MISC_QUADRATICFUNCTION_HH
#define DELAUNAY_MISC_QUADRATICFUNCTION_HH

#include "Function.hh"

namespace Delaunay
{
namespace Misc
{

class QuadraticFunction : public Function
{
public:
  QuadraticFunction();
  double operator() (double x) const;

  void SetCoefficients(double aa, double bb, double cc);
  void SetCoefficients(double* coeff);

protected:
  double coefficients[3];
};

}
}

#endif
