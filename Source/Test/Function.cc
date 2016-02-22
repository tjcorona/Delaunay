#include <opencv2/opencv.hpp>

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"
#include "Misc/QuadraticFunction.hh"

#include <iostream>

using namespace Delaunay;
using namespace Delaunay::Visualization;

class Sin : public Misc::Function
{
public:
  Sin(double alt=1., double freq=1., double off=0.) :
    altitude(alt), frequency(freq), offset(off) {}

  double operator () (double x) const
    { return offset + altitude*sin(frequency*x); }

private:
  double altitude;
  double frequency;
  double offset;
};

int main(int /*argc*/,char** /*argv*/)
{
  // create a canvas with x span from -5 to 5, and y span from -5 to 5
  Visualization::CVCanvas canvas(-5,5,-1.5,1.5);

  // create a quadratic function
  // DTQuadraticFunction x2;

  Sin sinFunc(1.,10.,0.);

  // draw function
  // canvas.Draw(x2,NULL,Red);
  canvas.Draw(sinFunc,NULL,Red);
  canvas.SetTimeDelay(0);
  canvas.Update();

  return 0;
}
