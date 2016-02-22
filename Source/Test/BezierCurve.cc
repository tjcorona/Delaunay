#include "Shape/Point.hh"
#include "Shape/BezierCurve.hh"

#include "Misc/Random.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include <iostream>

using namespace Delaunay;
using namespace Delaunay::Visualization;

int main(int /*argc*/,char** /*argv*/)
{
  // create a canvas with x span from -5 to 5, and y span from -5 to 5
  Visualization::CVCanvas canvas(0.,10.,0.,10.);

  // create a curve
  Shape::BezierCurve c;
  c.Closed(true);

  const unsigned nPoints = 5;

  // double radius = 4.;

  // for (unsigned i=0;i<nPoints;i++)
  // {
  //   double theta = 2.*M_PI*((double)i)/nPoints;
  //   DTPoint p(5.+radius*cos(theta),5.+radius*sin(theta));
  //   canvas.Draw(p,Black);
  //   c.AddPoint(p);
  // }

  for (unsigned i=0;i<nPoints;i++)
  {
    Shape::Point p(Misc::Random::GetInstance().Uniform(1000)/100.,
		   Misc::Random::GetInstance().Uniform(1000)/100.);
    canvas.Draw(p,Black);
    c.AddPoint(p);
  }

  // draw the curve
  canvas.Draw(c,Red);
  canvas.SetTimeDelay(0);
  canvas.Update();

  return 0;
}
