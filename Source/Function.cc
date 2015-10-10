#include <opencv2/opencv.hpp>

#include "DTColor.hh"
#include "DTQuadraticFunction.hh"
#include "DTCVCanvas.hh"

#include <iostream>

class Sin : public DTFunction
{
public:
  double operator () (double x) const { return sin(1.*x); }
};

int main(int /*argc*/,char** /*argv*/)
{
  // create a canvas with x span from -5 to 5, and y span from -5 to 5
  DTCVCanvas canvas(-5,5,-1.5,1.5);

  // create a quadratic function
  // DTQuadraticFunction x2;

  Sin sinFunc;

  // draw function
  // canvas.DrawFunction(x2,NULL,Red);
  canvas.DrawFunction(sinFunc,NULL,Red);
  canvas.SetTimeDelay(0);
  canvas.Update();

  return 0;
}
