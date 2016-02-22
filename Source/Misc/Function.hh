#ifndef DELAUNAY_MISC_FUNCTION_HH
#define DELAUNAY_MISC_FUNCTION_HH

namespace Delaunay
{
namespace Misc
{

class Function
{
public:
  virtual ~Function() {}

  virtual double operator() (double x) const = 0;
};

}
}

#endif
