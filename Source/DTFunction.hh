#ifndef DTFUNCTION_HH
#define DTFUNCTION_HH

class DTFunction
{
public:
  virtual ~DTFunction() {}

  virtual double operator() (double x) const = 0;
};


#endif /* DTFUNCTION_HH */
