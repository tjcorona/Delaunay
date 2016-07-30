#include <opencv2/opencv.hpp>

#include "Misc/Random.hh"

#include "Shape/Circle.hh"
#include "Shape/Triangle.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include <iostream>

using namespace Delaunay;
using namespace Delaunay::Visualization;

int main(int /*argc*/,char** /*argv*/)
{
  // create a canvas with x span from 0 to 10, and y span from 0 to 10
  Visualization::CVCanvas canvas(0.,10.,0.,10.);

  // create three vertices
  Shape::Triangle::Vertex A(Misc::Random::GetInstance().Uniform(1000)/100.,
			    Misc::Random::GetInstance().Uniform(1000)/100.);
  Shape::Triangle::Vertex B(Misc::Random::GetInstance().Uniform(1000)/100.,
			    Misc::Random::GetInstance().Uniform(1000)/100.);
  Shape::Triangle::Vertex C(Misc::Random::GetInstance().Uniform(1000)/100.,
			    Misc::Random::GetInstance().Uniform(1000)/100.);

  // Shape::Triangle::Vertex A(1.,1.);
  // Shape::Triangle::Vertex B(1.,9.);
  // Shape::Triangle::Vertex C(9.,1.);

  // create a triangle from these three verties
  Shape::Triangle triangle(A,B,C);

  // print the triangle's vertices
  std::cout<<"vertex A: "<<triangle.A<<std::endl;
  std::cout<<"vertex B: "<<triangle.B<<std::endl;
  std::cout<<"vertex C: "<<triangle.C<<std::endl;

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

    if (triangle.Contains(p))
      canvas.Draw(p,Green);
    else
      canvas.Draw(p,Black);
  }

  canvas.SetTimeDelay(0);
  canvas.Update();

  return 0;
}
