#include <opencv2/opencv.hpp>

#include "DTColor.hh"
#include "DTTriangle.hh"
#include "DTCVCanvas.hh"

#include "Random.hh"

#include <iostream>

int main(int /*argc*/,char** /*argv*/)
{
  // create a canvas with x span from 0 to 10, and y span from 0 to 10
  DTCVCanvas canvas(0.,10.,0.,10.);

  // create three vertices
  DTTriangle::Vertex A(Random::GetInstance().Uniform(1000)/100.,
  		       Random::GetInstance().Uniform(1000)/100.);
  DTTriangle::Vertex B(Random::GetInstance().Uniform(1000)/100.,
  		       Random::GetInstance().Uniform(1000)/100.);
  DTTriangle::Vertex C(Random::GetInstance().Uniform(1000)/100.,
  		       Random::GetInstance().Uniform(1000)/100.);

  // DTTriangle::Vertex A(1.,1.);
  // DTTriangle::Vertex B(1.,9.);
  // DTTriangle::Vertex C(9.,1.);
  
  // create a triangle from these three verties
  DTTriangle triangle(A,B,C);

  // print the triangle's vertices
  std::cout<<"vertex A: "<<triangle.A<<std::endl;
  std::cout<<"vertex B: "<<triangle.B<<std::endl;
  std::cout<<"vertex C: "<<triangle.C<<std::endl;
  
  // draw the three points onto the canvas
  canvas.DrawPoint(A,Green);
  canvas.DrawPoint(B,Green);
  canvas.DrawPoint(C,Green);

  // draw the triangle onto the canvas
  canvas.DrawTriangle(triangle,Green);

  // draw the circumcenter and circumcircle onto the canvas
  canvas.DrawPoint(triangle.circumcenter,Red);
  canvas.DrawCircle(triangle.circumcenter,triangle.circumradius,Red);

  // canvas.Update();

  for (unsigned i=0;i<100;i++)
  {
    DTPoint p(Random::GetInstance().Uniform(1000)/100.,
	      Random::GetInstance().Uniform(1000)/100.);

    if (triangle.Contains(p))
      canvas.DrawPoint(p,Green);
    else
      canvas.DrawPoint(p,Black);
  }

  canvas.Update();

  return 0;
}
