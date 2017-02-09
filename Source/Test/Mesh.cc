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

#include "Discretization/AddInteriorPoint.hh"
#include "Discretization/DiscretizePolygon.hh"
#include "Discretization/TestDelaunayCondition.hh"

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

  unsigned nPerimeterEdge = 1;

  Mesh::Mesh mesh;

  std::vector<Shape::Point> vertices;
  unsigned nPoly = 20;

  for (unsigned i=0;i<nPoly;i++)
    vertices.push_back(Shape::Point(5.+4.9*cos(2.*M_PI*i/nPoly),
				    5.+4.9*sin(2.*M_PI*i/nPoly)));
  Shape::Polygon polygon(vertices);

  Discretization::DiscretizePolygon discretize;
  discretize(polygon, mesh);

  Discretization::AddInteriorPoint addInteriorPoint;
  Discretization::TestDelaunayCondition testDelaunayCondition;

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

    addInteriorPoint(p,mesh);

    // Shape::Point p2(Misc::Random::GetInstance().Uniform(1000)/1000.*10,
    // 		    Misc::Random::GetInstance().Uniform(1000)/1000.*10);
    // discretizer.AddInteriorPoint(p2,mesh);

    Mesh::TriangleSet illegalTriangles;
    if (!testDelaunayCondition(illegalTriangles, mesh))
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
    addInteriorPoint(points[*it],mesh);
    Mesh::TriangleSet illegalTriangles;
    if (!testDelaunayCondition(illegalTriangles, mesh))
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
