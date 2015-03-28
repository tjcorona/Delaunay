#include <opencv2/opencv.hpp>

#include "DTColor.hh"
#include "DTPoint.hh"
#include "DTTriangle.hh"
#include "DTCVCanvas.hh"

#include <iostream>

int main(int /*argc*/,char** /*argv*/)
{
  // create a canvas with x span from 0 to 10, and y psan from 0 to 10
  DTCVCanvas canvas(0.,10.,0.,10.);

  // create three points
  DTPoint A(5.,9.);
  DTPoint B(1,4.);
  DTPoint C(9.,3.);

  // create a triangle from these three points
  DTTriangle triangle(A,B,C);

  // draw the three points onto the canvas
  canvas.DrawPoint(A,Green);
  canvas.DrawPoint(B,Green);
  canvas.DrawPoint(C,Green);

  // draw the triangle onto the canvas
  canvas.DrawTriangle(triangle,Green);

  // draw the circumcenter and circumcircle onto the canvas
  canvas.DrawPoint(triangle.circumcenter,Black);
  canvas.DrawCircle(triangle.circumcenter,triangle.circumradius,Black);

  canvas.Update();

  return 0;
}
