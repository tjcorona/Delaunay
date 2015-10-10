#include "DTColor.hh"
#include "DTPoint.hh"
#include "DTBezierCurve.hh"
#include "DTCVCanvas.hh"
#include "Random.hh"

#include <iostream>

int main(int /*argc*/,char** /*argv*/)
{
  // create a canvas with x span from -5 to 5, and y span from -5 to 5
  DTCVCanvas canvas(0.,10.,0.,10.);

  // create a curve
  DTBezierCurve c;
  c.Closed(true);
  
  const unsigned nPoints = 5;

  // double radius = 4.;

  // for (unsigned i=0;i<nPoints;i++)
  // {
  //   double theta = 2.*M_PI*((double)i)/nPoints;
  //   DTPoint p(5.+radius*cos(theta),5.+radius*sin(theta));
  //   canvas.DrawPoint(p,Black);
  //   c.AddPoint(p);
  // }
  
  for (unsigned i=0;i<nPoints;i++)
  {
    DTPoint p(Random::GetInstance().Uniform(1000)/100.,
              Random::GetInstance().Uniform(1000)/100.);
    canvas.DrawPoint(p,Black);
    c.AddPoint(p);
  }

  // draw the curve
  canvas.DrawParametricCurve(c,Red);
  canvas.SetTimeDelay(0);
  canvas.Update();

  return 0;
}
