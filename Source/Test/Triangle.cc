#include <opencv2/opencv.hpp>

#include "Misc/Random.hh"

#include "Shape/Circle.hh"
#include "Shape/Triangle.hh"
#include "Shape/TriangleUtilities.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include <iostream>

using namespace Delaunay;
using namespace Delaunay::Visualization;

int main(int /*argc*/,char** /*argv*/)
{
  // create a canvas with x span from 0 to 10, and y span from 0 to 10
  Visualization::CVCanvas canvas(0.,10.,0.,10.);

  // create three points
  Shape::Point A(Misc::Random::GetInstance().Uniform(1000)/100.,
		 Misc::Random::GetInstance().Uniform(1000)/100.);
  Shape::Point B(Misc::Random::GetInstance().Uniform(1000)/100.,
		 Misc::Random::GetInstance().Uniform(1000)/100.);
  Shape::Point C(Misc::Random::GetInstance().Uniform(1000)/100.,
		 Misc::Random::GetInstance().Uniform(1000)/100.);

  // create three line segments from these three points
  Shape::LineSegment AB(A,B);
  Shape::LineSegment BC(B,C);
  Shape::LineSegment AC(A,C);

  // create a triangle from these three line segments
  Shape::Triangle triangle(AB,BC,AC);

  // print the triangle's line segments
  std::cout<<"line segment AB: "<<triangle.AB<<std::endl;
  std::cout<<"line segment BC: "<<triangle.BC<<std::endl;
  std::cout<<"line segment AC: "<<triangle.AC<<std::endl;

  // draw the three points onto the canvas
  canvas.Draw(A,Green);
  canvas.Draw(B,Green);
  canvas.Draw(C,Green);

  // draw the triangle onto the canvas
  canvas.Draw(triangle,Green);

  // draw the circumcenter and circumcircle onto the canvas
  canvas.Draw(triangle.circumcenter,Red);
  Color faintRed(255,0,0,128);
  canvas.Draw(Shape::Circle(triangle.circumcenter,triangle.circumradius),Red,faintRed);

  // canvas.Update();

  for (unsigned i=0;i<100;i++)
  {
    Shape::Point p(Misc::Random::GetInstance().Uniform(1000)/100.,
		   Misc::Random::GetInstance().Uniform(1000)/100.);

    if (Contains(triangle,p))
      canvas.Draw(p,Green);
    else
      canvas.Draw(p,Black);
  }

  canvas.SetTimeDelay(0);
  canvas.Update();

  return 0;
}
