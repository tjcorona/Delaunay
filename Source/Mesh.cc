#include <opencv2/opencv.hpp>

#include "DTColor.hh"
#include "DTTriangle.hh"
#include "DTCVCanvas.hh"
#include "DTDelaunayMesh.hh"

#include "Random.hh"

#include <iostream>

void DrawMesh(const DTMesh& mesh,
	      std::set<const DTTriangle*> illegalTriangles,
	      DTCVCanvas& canvas);

int main(int /*argc*/,char** /*argv*/)
{
  // create a canvas with x span from 0 to 10, and y span from 0 to 10

  double x_min,y_min,x_max,y_max;
  x_min = y_min = 0.;
  x_max = y_max = 10.;

  DTPoint p0(x_min,y_min);
  DTPoint p1(x_min,y_max);
  DTPoint p2(x_max,y_min);
  DTPoint p3(x_max,y_max);

  DTCVCanvas canvas(x_min,x_max,y_min,y_max);
  canvas.SetTimeDelay(1.e-2);
  // canvas.SetTimeDelay(5.e-1);

  unsigned nPerimeterEdge = 1;

  DTDelaunayMesh mesh;
  // mesh.AddPerimeterPoint(p0);
  // mesh.AddPerimeterPoint(p1);
  // mesh.AddPerimeterPoint(p2);
  // mesh.AddPerimeterPoint(p3);

/*
  mesh.AddPerimeterPoint(p0);
  for (unsigned i=0;i<nPerimeterEdge;i++)
    mesh.AddPerimeterPoint(DTPoint(x_min,y_min + (y_max - y_min)*i));
  mesh.AddPerimeterPoint(p1);
  for (unsigned i=0;i<nPerimeterEdge;i++)
    mesh.AddPerimeterPoint(DTPoint(x_min + (x_max - x_min)*i,y_max));
  mesh.AddPerimeterPoint(p2);
  for (unsigned i=0;i<nPerimeterEdge;i++)
    mesh.AddPerimeterPoint(DTPoint(x_min,y_min + (y_max - y_min)*(nPerimeterEdge - 1 - i)));
  mesh.AddPerimeterPoint(p3);
  for (unsigned i=0;i<nPerimeterEdge;i++)
    mesh.AddPerimeterPoint(DTPoint(x_min + (x_max - x_min)*(nPerimeterEdge - 1 - i),y_min));
*/

  unsigned nPoly = 20;

  for (unsigned i=0;i<nPoly;i++)
  {
    // mesh.AddInteriorPoint(DTPoint(5.+4.9*cos(2.*M_PI*i/nPoly),
    // 				  5.+4.9*sin(2.*M_PI*i/nPoly)));
    mesh.AddPerimeterPoint(DTPoint(5.+4.9*cos(2.*M_PI*i/nPoly),
    				   5.+4.9*sin(2.*M_PI*i/nPoly)));

    std::set<const DTTriangle*> illegalTriangles;
    if (!mesh.TestDelaunayCondition(illegalTriangles))
      std::cout<<"Failed"<<std::endl;
    DrawMesh(mesh,illegalTriangles,canvas);
  }

  // for (unsigned i=0;i<nPoly;i++)
  // {
  //   mesh.AddInteriorPoint(DTPoint(5.+2.5*cos(2.*M_PI*i/nPoly + M_PI),
  // 				  5.+2.5*sin(2.*M_PI*i/nPoly + M_PI)));

  //   std::set<const DTTriangle*> illegalTriangles;
  //   if (!mesh.TestDelaunayCondition(illegalTriangles))
  //     std::cout<<"Failed"<<std::endl;
  //   DrawMesh(mesh,illegalTriangles,canvas);
  // }

  // Random::GetInstance().Seed(0);

  // for (unsigned i=0;i<500;i++)
  // {
  //   DTPoint p(x_min + Random::GetInstance().Uniform(1000)/1000.*(x_max-x_min),
  // 	      y_min + Random::GetInstance().Uniform(1000)/1000.*(y_max-y_min));

  //   mesh.AddInteriorPoint(p);

  //   std::set<const DTTriangle*> illegalTriangles;
  //   if (!mesh.TestDelaunayCondition(illegalTriangles))
  //     std::cout<<"Failed"<<std::endl;
  //   DrawMesh(mesh,illegalTriangles,canvas);
  // }

  for (unsigned i=0;i<1000;i++)
  {
    // uniform distribution
    double r = 4.9*pow(Random::GetInstance().Uniform(1000)/1000.,.5);
    // center weighted distribution
    // double r = 4.9*pow(Random::GetInstance().Uniform(1000)/1000.,2.);
    // edge weighted distribution
    // double r = 4.9*pow(Random::GetInstance().Uniform(1000)/1000.,.25);
    double theta = 2.*M_PI*Random::GetInstance().Uniform(1000)/1000.;

    DTPoint p(5. + r*cos(theta), 5. + r*sin(theta));

    mesh.AddInteriorPoint(p);

    std::set<const DTTriangle*> illegalTriangles;
    if (!mesh.TestDelaunayCondition(illegalTriangles))
      std::cout<<"Failed"<<std::endl;
    DrawMesh(mesh,illegalTriangles,canvas);
  }

  // std::vector<DTPoint> points;
  // std::vector<int> pointIndex;
  // unsigned counter = 0;

  // unsigned nGridPoints = 10;

  // for (unsigned i=0;i<nGridPoints;i++)
  // {
  //   for (unsigned j=0;j<nGridPoints;j++)
  //   {
  //     points.push_back(DTPoint(x_min + ((i+.5)/nGridPoints)*(x_max-x_min),
  // 			       y_min + ((j+.5)/nGridPoints)*(y_max-y_min)));

  //     pointIndex.push_back(counter++);
  //   }
  // }

  // Random::GetInstance().Shuffle(pointIndex);

  // for (std::vector<int>::iterator it=pointIndex.begin();it!=pointIndex.end();++it)
  // {
  //   mesh.AddInteriorPoint(points[*it]);
  //   std::set<const DTTriangle*> illegalTriangles;
  //   if (!mesh.TestDelaunayCondition(illegalTriangles))
  //     std::cout<<"Failed"<<std::endl;
  //   DrawMesh(mesh,illegalTriangles,canvas);
  // }

  return 0;
}

void DrawMesh(const DTMesh& mesh,
	      std::set<const DTTriangle*> illegalTriangles,
	      DTCVCanvas& canvas)
{
  const DTMesh::TriangleSet& triangles = mesh.GetTriangles();
  const DTMesh::VertexSet& vertices = mesh.GetVertices();

  for (DTMesh::TriangleSet::const_iterator it=triangles.begin();it!=triangles.end();++it)
  {
    // canvas.DrawCircle((*it)->circumcenter,(*it)->circumradius,Red);
    canvas.DrawTriangle(*(*it),Black);
  }

  for (std::set<const DTTriangle*>::const_iterator it=illegalTriangles.begin();it!=illegalTriangles.end();++it)
  {
    canvas.DrawTriangle(*(*it),Red);
  }

  for (DTMesh::VertexSet::const_iterator it=vertices.begin();it!=vertices.end();++it)
  {
    canvas.DrawPoint(*(*it),Black);
  }

  canvas.Update();
}
