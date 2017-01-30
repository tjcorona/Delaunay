#include <opencv2/opencv.hpp>

#include "Discretization/DelaunayDiscretizer.hh"

#include "Misc/Random.hh"

#include "Shape/Triangle.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include <iostream>

using namespace Delaunay;
using namespace Delaunay::Visualization;

void Draw(const Mesh::Mesh& mesh,
	  Mesh::TriangleSet& illegalTriangles,
	  Visualization::CVCanvas& canvas);

int main(int /*argc*/,char** /*argv*/)
{
  // create a canvas with x span from 0 to 10, and y span from 0 to 10

  double x_min,y_min,x_max,y_max;
  x_min = y_min = 0.;
  x_max = y_max = 10.;

  Shape::Point p0(x_min,y_min);
  Shape::Point p1(x_max,y_min);
  Shape::Point p2(x_max,y_max);
  Shape::Point p3(x_min,y_max);

  Visualization::CVCanvas canvas(x_min,x_max,y_min,y_max);
  canvas.SetTimeDelay(1.e-2);
  // canvas.SetTimeDelay(5.e-1);

  unsigned nPerimeterEdge = 1;

  Mesh::Mesh mesh;
  Discretization::DelaunayDiscretizer discretizer;
  // discretizer.AddPerimeterPoint(p0,mesh);
  // discretizer.AddPerimeterPoint(p1,mesh);
  // discretizer.AddPerimeterPoint(p2,mesh);
  // discretizer.AddPerimeterPoint(p3,mesh);

  std::vector<Shape::Point> vertices;
  // vertices.push_back(p0);
  // vertices.push_back(p1);
  // vertices.push_back(p2);
  // vertices.push_back(p3);
  // Shape::Polygon polygon(vertices);

  // discretizer.Mesh(polygon, mesh);

  // discretizer.AddPerimeterPoint(p0,mesh);
  // for (unsigned i=0;i<nPerimeterEdge;i++)
  //   discretizer.AddPerimeterPoint(Shape::Point(x_min,y_min + (y_max - y_min)*i),mesh);
  // discretizer.AddPerimeterPoint(p1,mesh);
  // for (unsigned i=0;i<nPerimeterEdge;i++)
  //   discretizer.AddPerimeterPoint(Shape::Point(x_min + (x_max - x_min)*i,y_max),mesh);
  // discretizer.AddPerimeterPoint(p2,mesh);
  // for (unsigned i=0;i<nPerimeterEdge;i++)
  //   discretizer.AddPerimeterPoint(Shape::Point(x_min,y_min + (y_max - y_min)*(nPerimeterEdge - 1 - i)),mesh);
  // discretizer.AddPerimeterPoint(p3,mesh);
  // for (unsigned i=0;i<nPerimeterEdge;i++)
  //   discretizer.AddPerimeterPoint(Shape::Point(x_min + (x_max - x_min)*(nPerimeterEdge - 1 - i),y_min),mesh);

  unsigned nPoly = 20;

  for (unsigned i=0;i<nPoly;i++)
  {
    // discretizer.AddInteriorPoint(Shape::Point(5.+4.9*cos(2.*M_PI*i/nPoly),
    // 					      5.+4.9*sin(2.*M_PI*i/nPoly)),
    // 				 mesh);
    // discretizer.AddPerimeterPoint(Shape::Point(5.+4.9*cos(2.*M_PI*i/nPoly),
    // 					       5.+4.9*sin(2.*M_PI*i/nPoly)),
    // 				  mesh);
    vertices.push_back(Shape::Point(5.+4.9*cos(2.*M_PI*i/nPoly),
				    5.+4.9*sin(2.*M_PI*i/nPoly)));

    // Mesh::TriangleSet illegalTriangles;
    // if (!discretizer.TestDelaunayCondition(illegalTriangles, mesh))
    //   std::cout<<"Failed"<<std::endl;
    // Draw(mesh,illegalTriangles,canvas);
  }
  Shape::Polygon polygon(vertices);
  discretizer.Mesh(polygon, mesh);

  // for (unsigned i=0;i<nPoly;i++)
  // {
  //   mesh.AddInteriorPoint(Shape::Point(5.+2.5*cos(2.*M_PI*i/nPoly + M_PI),
  // 				  5.+2.5*sin(2.*M_PI*i/nPoly + M_PI)));

  //   std::set<const Shape::Triangle*> illegalTriangles;
  //   if (!mesh.TestDelaunayCondition(illegalTriangles))
  //     std::cout<<"Failed"<<std::endl;
  //   Draw(mesh,illegalTriangles,canvas);
  // }

  // Misc::Random::GetInstance().Seed(0);

  // for (unsigned i=0;i<500;i++)
  // {
  //   Shape::Point p(x_min + Misc::Random::GetInstance().Uniform(1000)/1000.*(x_max-x_min),
  // 	      y_min + Misc::Random::GetInstance().Uniform(1000)/1000.*(y_max-y_min));

  //   mesh.AddInteriorPoint(p);

  //   std::set<const Shape::Triangle*> illegalTriangles;
  //   if (!mesh.TestDelaunayCondition(illegalTriangles))
  //     std::cout<<"Failed"<<std::endl;
  //   Draw(mesh,illegalTriangles,canvas);
  // }

  for (unsigned i=0;i<1000;i++)
  {
    // uniform distribution
    // double r = 4.9*pow(Misc::Random::GetInstance().Uniform(1000)/1000.,.5);
    double r = 5.5*pow(Misc::Random::GetInstance().Uniform(1000)/1000.,.5);
    // center weighted distribution
    // double r = 4.9*pow(Misc::Random::GetInstance().Uniform(1000)/1000.,2.);
    // edge weighted distribution
    // double r = 4.9*pow(Misc::Random::GetInstance().Uniform(1000)/1000.,.25);
    double theta = 2.*M_PI*Misc::Random::GetInstance().Uniform(1000)/1000.;

    Shape::Point p(5. + r*cos(theta), 5. + r*sin(theta));

    discretizer.AddInteriorPoint(p,mesh);

    // Shape::Point p2(Misc::Random::GetInstance().Uniform(1000)/1000.*10,
    // 		    Misc::Random::GetInstance().Uniform(1000)/1000.*10);
    // discretizer.AddInteriorPoint(p2,mesh);

    Mesh::TriangleSet illegalTriangles;
    if (!discretizer.TestDelaunayCondition(illegalTriangles, mesh))
      std::cout<<"Failed"<<std::endl;
    Draw(mesh,illegalTriangles,canvas);
  }

  std::vector<Shape::Point> points;
  std::vector<int> pointIndex;
  unsigned counter = 0;

  unsigned nGridPoints = 10;

  for (unsigned i=0;i<nGridPoints;i++)
  {
    for (unsigned j=0;j<nGridPoints;j++)
    {
      points.push_back(Shape::Point(x_min + ((i+.5)/nGridPoints)*(x_max-x_min),
  			       y_min + ((j+.5)/nGridPoints)*(y_max-y_min)));

      pointIndex.push_back(counter++);
    }
  }

  Misc::Random::GetInstance().Shuffle(pointIndex);

  for (std::vector<int>::iterator it=pointIndex.begin();it!=pointIndex.end();++it)
  {
    discretizer.AddInteriorPoint(points[*it],mesh);
    Mesh::TriangleSet illegalTriangles;
    if (!discretizer.TestDelaunayCondition(illegalTriangles, mesh))
      std::cout<<"Failed"<<std::endl;
    Draw(mesh,illegalTriangles,canvas);
  }

  return 0;
}

void Draw(const Mesh::Mesh& mesh,
	  Mesh::TriangleSet& illegalTriangles,
	  Visualization::CVCanvas& canvas)
{
  const Mesh::Mesh::TriangleSet& triangles = mesh.GetTriangles();
  const Mesh::Mesh::VertexSet& vertices = mesh.GetVertices();

  for (auto it=triangles.begin();it!=triangles.end();++it)
  {
    // canvas.Draw((*it)->circumcenter,(*it)->circumradius,Red);
    canvas.Draw(*it,Black);
  }

  for (auto it=illegalTriangles.begin();it!=illegalTriangles.end();++it)
  {
    canvas.Draw(*(*it),Red);
  }

  for (auto it=vertices.begin();it!=vertices.end();++it)
  {
    canvas.Draw(*it,Black);
  }

  canvas.Update();
}
