#ifndef DTPARAMETRICCURVE_HH
#define DTPARAMETRICCURVE_HH

class DTPoint;

class DTParametricCurve
{
public:
  virtual ~DTParametricCurve() {}

  virtual const DTPoint operator() (double) const = 0;

  void Closed(bool choice) { closed = choice; }
  bool Closed() const { return closed; }

private:
  bool closed;
};


#endif /* DTPARAMETRICCURVE_HH */
