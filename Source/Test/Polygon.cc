/******************************************************************************

  This source file is part of the Delaunay project.

  Copyright T.J. Corona

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#include <opencv2/opencv.hpp>

#include "Shape/LineSegment.hh"
#include "Shape/Polygon.hh"
#include "Shape/Point.hh"

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

void RandomPolygon(int nPoints,
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
  for (unsigned i=0;i<nPoints;i++)
  {
    vertices.push_back(Coord(5. + 2.5*cos(i*2.*M_PI/nPoints),
			     5. + 2.5*sin(i*2.*M_PI/nPoints)));
    direction.push_back((Misc::Random::GetInstance().Uniform(1000)/1000.)*(2.*M_PI));
    velocity.push_back((Misc::Random::GetInstance().Uniform(1000)/1000.)*.25);
  }

  for (unsigned step=0;step<nSteps;step++)
  {
    for (unsigned i=0;i<nPoints;i++)
    {
      Coord c(vertices[i].x + velocity[i]*cos(direction[i]),
	      vertices[i].y + velocity[i]*sin(direction[i]));

      unsigned last = (i + nPoints - 1) % nPoints;
      unsigned next = (i + 1) % nPoints;

      bool valid = true;
      for (unsigned j=0;j<nPoints;j++)
      {
	if (last == j || i == j)
	  continue;

	unsigned n = (j + 1) % nPoints;

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
  PolygonType testType = Random;

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
    default:
      assert(false);
  }

  // create a canvas with x span from 0 to 10, and y span from 0 to 10
  double bounds[4] = {0.,10.,0.,10.};
  Visualization::CVCanvas canvas(bounds[0],bounds[1],bounds[2],bounds[3]);

  const unsigned nPoints = 3 + Misc::Random::GetInstance().Uniform(100);
  std::vector<Shape::Point> vertices;

  if (testType == Regular)
  {
    double theta = 0.;
    for (unsigned i=0;i<nPoints;i++)
    {
      vertices.push_back(Shape::Point(5. + 4.*cos(theta), 5. + 4.*sin(theta)));
      theta += 2.*M_PI/nPoints;
    }
  }
  else if (testType == StarConvex)
    {
      std::vector<double> thetas(nPoints);
      std::vector<double> radii(nPoints);
      bool sharedTheta = true;
      while (sharedTheta)
      {
	sharedTheta = false;
	double min_radius =.1+3.9*(Misc::Random::GetInstance().Uniform(10000)/10000.);
	double max_radius = 5. - min_radius;
	for (unsigned i=0;i<nPoints;i++)
	{
	  radii[i] = (min_radius +
		      max_radius*(Misc::Random::GetInstance().Uniform(10000)/10000.));
	  thetas[i] = Misc::Random::GetInstance().Uniform(360)/180.*M_PI;
	}
	std::sort(thetas.begin(),thetas.end());

	unsigned i_next;
	for (unsigned i=0;i<nPoints;i++)
	{
	  i_next = (i+1)%nPoints;
	  if (fabs(thetas[i]-thetas[i_next]) < 1.e-6)
	  {
	    sharedTheta = true;
	    break;
	  }
	}
      }

      for (unsigned i=0;i<nPoints;i++)
	vertices.push_back(Shape::Point(5. + radii[i]*cos(thetas[i]),
				   5. + radii[i]*sin(thetas[i])));
    }
  else if (testType == RandomEvolve)
  {
    std::vector<Coord> verts;
    RandomPolygon(nPoints,verts,bounds,
		  Misc::Random::GetInstance().Uniform(50));
    for (unsigned i=0;i<nPoints;i++)
      vertices.push_back(Shape::Point(verts[i].x,verts[i].y));
  }

  // create a polygon from the point vector
  Shape::Polygon polygon(vertices);

  Color faintRed(255,0,0,128);

  canvas.Draw(polygon,Red,faintRed);

  for (unsigned i=0;i<polygon.GetPoints().size();i++)
    canvas.Draw(vertices[i],
  		Visualization::Color(i/(polygon.GetPoints().size()-1.),
  				     Visualization::Color::BlueToRed));

  canvas.SetTimeDelay(0.);
  canvas.Update();

  return 0;
}
