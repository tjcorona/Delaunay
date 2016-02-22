#include <opencv2/opencv.hpp>

#include "Shape/Polygon.hh"
#include "Shape/Triangle.hh"

#include "Misc/Random.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include <algorithm>
#include <iostream>

using namespace Delaunay;
using namespace Delaunay::Visualization;

class Coord
{
public:
  Coord() : x(0.), y(0.) {}
  Coord(double xx,double yy) : x(xx), y(yy) {}

  double x;
  double y;

  friend Coord operator*(double d,const Coord& p)
  {
    return Coord(p.x*d,p.y*d);
  }

  friend Coord operator*(const Coord& p,double d)
  {
    return operator*(d,p);
  }

  friend Coord operator/(const Coord& p,double d)
  {
    return Coord(p.x/d,p.y/d);
  }

  friend Coord operator+(const Coord& p1,const Coord& p2)
  {
    return Coord(p1.x + p2.x,p1.y + p2.y);
  }

  friend Coord operator-(const Coord& p1,const Coord& p2)
  {
    return Coord(p1.x - p2.x,p1.y - p2.y);
  }
};

int Orientation(const Coord& p1, const Coord& p2, const Coord& p3)
{
  // +1: counterclockwise
  //  0: colinear
  // -1: clockwise
  Coord v1 = p2 - p1;
  Coord v2 = p3 - p1;

  double cross = v1.x*v2.y - v1.y*v2.x;
  if (fabs(cross) < EPSILON)
    return 0;
  else
    return (cross > 0. ? 1 : -1);
}

bool OnLineSegment(const Coord& p, const Coord& p1, const Coord& p2)
{
  // Assuming colinearity, does p fall on the line segment connecting p1 and
  // p2?
  if (fabs(p1.x - p.x) > EPSILON)
    return (p1.x < p.x && p2.x > p.x) || (p2.x < p.x && p1.x > p.x);
  else
    return (p1.y < p.y && p2.y > p.y) || (p2.y < p.y && p1.y > p.y);
  }

bool Intersect(const Coord& p1, const Coord& p2,
	       const Coord& m1, const Coord& m2)
{
  // Does line segment (p1,p2) intersect with line segment (m1,m2)?
  int orientation[4] = {Orientation(m1,m2,p1),Orientation(m1,m2,p2),
			Orientation(p1,p2,m1),Orientation(p1,p2,m2)};

  if (orientation[0] == 0)
    return OnLineSegment(p1,m1,m2);
  if (orientation[1] == 0)
    return OnLineSegment(p2,m1,m2);
  if (orientation[2] == 0)
    return OnLineSegment(m1,p1,p2);
  if (orientation[3] == 0)
    return OnLineSegment(m2,p1,p2);

  return (orientation[0]*orientation[1] == -1 &&
	  orientation[2]*orientation[3] == -1);
}

void RandomPolygon(int nVertices,
		   std::vector<Coord>& vertices,
		   double* bounds,
		   unsigned nSteps)
{
  // Generating Random Polygons
  // Joseph O'Rourke & Mandira Virmani
  // July 25, 1991
  // http://cs.smith.edu/~orourke/Papers/GenRandPoly.pdf

  Coord b0(bounds[0],bounds[2]);
  Coord b1(bounds[0],bounds[3]);
  Coord b2(bounds[1],bounds[2]);
  Coord b3(bounds[1],bounds[3]);

  std::vector<double> direction;
  std::vector<double> velocity;
  for (unsigned i=0;i<nVertices;i++)
  {
    vertices.push_back(Coord(5. + 2.5*cos(i*2.*M_PI/nVertices),
			     5. + 2.5*sin(i*2.*M_PI/nVertices)));
    direction.push_back((Misc::Random::GetInstance().Uniform(1000)/1000.)*(2.*M_PI));
    velocity.push_back((Misc::Random::GetInstance().Uniform(1000)/1000.)*.25);
  }

  for (unsigned step=0;step<nSteps;step++)
  {
    for (unsigned i=0;i<nVertices;i++)
    {
      Coord c(vertices[i].x + velocity[i]*cos(direction[i]),
	      vertices[i].y + velocity[i]*sin(direction[i]));

      unsigned last = (i + nVertices - 1) % nVertices;
      unsigned next = (i + 1) % nVertices;

      bool valid = true;
      for (unsigned j=0;j<nVertices;j++)
      {
	if (last == j || i == j)
	  continue;

	unsigned n = (j + 1) % nVertices;

	if (Intersect(vertices[last],c,vertices[j],vertices[n]) ||
	    Intersect(vertices[next],c,vertices[j],vertices[n]))
	{
	  valid = false;
	  break;
	}
      }

      if (valid)
      {
	valid &= !Intersect(vertices[last],c,b0,b1);
	valid &= !Intersect(vertices[last],c,b1,b3);
	valid &= !Intersect(vertices[last],c,b3,b2);
	valid &= !Intersect(vertices[last],c,b2,b0);

	valid &= !Intersect(vertices[next],c,b0,b1);
	valid &= !Intersect(vertices[next],c,b1,b3);
	valid &= !Intersect(vertices[next],c,b3,b2);
	valid &= !Intersect(vertices[next],c,b2,b0);
      }

      if (valid)
      {
	vertices[i].x = c.x;
	vertices[i].y = c.y;
      }
      else
      {
	direction[i] = (Misc::Random::GetInstance().Uniform(1000)/1000.)*(2.*M_PI);
      }
    }
  }
}

enum PolygonType
{
  Regular,
  StarConvex,
  RandomEvolve,
  Random
};


int main(int argc,char** argv)
{
  PolygonType testType = Regular;

  if (argc > 1)
    testType = static_cast<PolygonType>(atoi(argv[1]));

  if (testType < 0 || testType > 3)
  {
    std::cout<<"Polygon: possible options:"<<std::endl;
    std::cout<<"         0: Regular (default)"<<std::endl;
    std::cout<<"         1: Star Convex"<<std::endl;
    std::cout<<"         2: Random Evolve"<<std::endl;
    std::cout<<"         3: Random"<<std::endl;
    return 1;
  }

  if (testType == Random)
    testType = static_cast<PolygonType>(Misc::Random::GetInstance().Uniform(3));

  std::cout<<"Test type: ";
  switch (testType)
  {
    case Regular:
      std::cout<<"Regular"<<std::endl;
      break;
    case StarConvex:
      std::cout<<"Star Convex"<<std::endl;
      break;
    case RandomEvolve:
      std::cout<<"RandomEvolve"<<std::endl;
      break;
  }

  // create a canvas with x span from 0 to 10, and y span from 0 to 10
  double bounds[4] = {0.,10.,0.,10.};
  Visualization::CVCanvas canvas(bounds[0],bounds[1],bounds[2],bounds[3]);

  const unsigned nVertices = 3 + Misc::Random::GetInstance().Uniform(100);
  std::vector<Shape::Point> vertices;

  if (testType == Regular)
  {
    double theta = 0.;
    for (unsigned i=0;i<nVertices;i++)
    {
      vertices.push_back(Shape::Point(5. + 4.*cos(theta), 5. + 4.*sin(theta)));
      theta += 2.*M_PI/nVertices;
    }
  }
  else if (testType == StarConvex)
    {
      std::vector<double> thetas(nVertices);
      std::vector<double> radii(nVertices);
      bool sharedTheta = true;
      while (sharedTheta)
      {
	sharedTheta = false;
	double min_radius =.1+3.9*(Misc::Random::GetInstance().Uniform(10000)/10000.);
	double max_radius = 5. - min_radius;
	for (unsigned i=0;i<nVertices;i++)
	{
	  radii[i] = (min_radius +
		      max_radius*(Misc::Random::GetInstance().Uniform(10000)/10000.));
	  thetas[i] = Misc::Random::GetInstance().Uniform(360)/180.*M_PI;
	}
	std::sort(thetas.begin(),thetas.end());

	unsigned i_next;
	for (unsigned i=0;i<nVertices;i++)
	{
	  i_next = (i+1)%nVertices;
	  if (fabs(thetas[i]-thetas[i_next]) < 1.e-6)
	  {
	    sharedTheta = true;
	    break;
	  }
	}
      }

      for (unsigned i=0;i<nVertices;i++)
	vertices.push_back(Shape::Point(5. + radii[i]*cos(thetas[i]),
				   5. + radii[i]*sin(thetas[i])));
    }
  else if (testType == RandomEvolve)
  {
    std::vector<Coord> verts;
    RandomPolygon(nVertices,verts,bounds,
		  Misc::Random::GetInstance().Uniform(50));
    for (unsigned i=0;i<nVertices;i++)
      vertices.push_back(Shape::Point(verts[i].x,verts[i].y));
  }

  // for (unsigned i=0;i<vertices.size();i++)
  //   std::cout<<"vertices.push_back(Shape::Point("<<vertices[i].x<<","<<vertices[i].y<<"));"<<std::endl;
  // std::cout<<""<<std::endl;

  // create a triangle from these three verties
  Shape::Polygon polygon(vertices);

  std::vector<Shape::Triangle*> triangles;
  bool success = polygon.Triangulate(triangles);

  if (!success)
    std::cout<<"Failed to triangulate"<<std::endl;

  for (unsigned i=0;i<triangles.size();i++)
  {
    canvas.Draw(*triangles[i],Green);
    canvas.Draw((triangles[i]->A + triangles[i]->B + triangles[i]->C)/3.,
		     Green);
  }

  canvas.Draw(polygon,Red);

  for (unsigned i=0;i<polygon.Vertices.size();i++)
    canvas.Draw(vertices[i],
		     Visualization::Color(i/(polygon.Vertices.size()-1.),
					  Visualization::Color::BlueToRed));

  canvas.SetTimeDelay(1.);
  if (!success)
    canvas.SetTimeDelay(0.);
  canvas.Update();

  return 0;
}
