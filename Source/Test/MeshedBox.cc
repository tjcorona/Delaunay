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

#include "Mesh/Mesh.hh"

#include "Discretization/DelaunayDiscretizer.hh"
#include "Discretization/PolygonDiscretizer.hh"
#include "Discretization/PolygonExciser.hh"

#include "Misc/Random.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include <algorithm>
#include <iostream>

#include "PolygonGenerator.cc"

using namespace Delaunay;
using namespace Delaunay::Visualization;

int main(int argc,char** argv)
{
  if (argc > 1)
    Misc::Random::GetInstance().Seed(atoi(argv[1]));

  // create a canvas with x span from 0 to 10, and y span from 0 to 10
  double bounds[4] = {0.,10.,0.,10.};
  Visualization::CVCanvas canvas(bounds[0],bounds[1],bounds[2],bounds[3]);

  std::vector<Shape::Point> vertices;

  for (std::size_t i=0;i<1;i++)
  {
    vertices.push_back(Shape::Point((bounds[1]*i + bounds[0]*(10.-i))/10.,
				    bounds[2]));
  }

  for (std::size_t i=0;i<1;i++)
  {
    vertices.push_back(Shape::Point(bounds[1],
				    (bounds[3]*i + bounds[2]*(10.-i))/10.));
  }

  for (std::size_t i=0;i<1;i++)
  {
    vertices.push_back(Shape::Point((bounds[0]*i + bounds[1]*(10.-i))/10.,
				    bounds[3]));
  }

  for (std::size_t i=0;i<1;i++)
  {
    vertices.push_back(Shape::Point(bounds[0],
				    (bounds[2]*i + bounds[3]*(10.-i))/10.));
  }

  // create a polygon from the point vector
  Shape::Polygon polygon(vertices);

  std::set<const Shape::Polygon> interiorBoundaries;
  std::vector<std::vector<Shape::Point> > interiorPoints;

  {
    // Step 1: cut a hole in the box
    unsigned nx = 2*(1 + Misc::Random::GetInstance().Uniform(5));
    unsigned ny = 2*(1 + Misc::Random::GetInstance().Uniform(5));

    nx = ny = 2;

    std::deque<double> x(1,0.);
    for (unsigned i=0; i<nx+1; i++)
      x.push_back(x.back() + 10. + Misc::Random::GetInstance().Uniform(10));
    x.pop_front();
    for (unsigned i=0; i<nx; i++)
      x[i] /= x.back();
    x.pop_back();

    nx /= 2;

    std::deque<double> y(1,0);
    for (unsigned i=0; i<ny+1; i++)
      y.push_back(y.back() + 10. + Misc::Random::GetInstance().Uniform(10));
    y.pop_front();
    for (unsigned i=0; i<ny; i++)
      y[i] /= y.back();
    y.pop_back();

    ny /= 2;

    for (unsigned i=0; i<nx; i++)
    {
      for (unsigned j=0; j<ny; j++)
      {
	double subBounds[4] = {bounds[0] + (bounds[1] - bounds[0])*x[i],
			       bounds[0] + (bounds[1] - bounds[0])*x[i+1],
			       bounds[2] + (bounds[3] - bounds[2])*y[j],
			       bounds[2] + (bounds[3] - bounds[2])*y[j+1]};
	// const unsigned nPoints = 3 + Misc::Random::GetInstance().Uniform(50);
        const unsigned nPoints = 3;
	// PolygonType polyType = static_cast<PolygonType>(
	//   Misc::Random::GetInstance().Uniform(3));
	PolygonType polyType = static_cast<PolygonType>(0);

	std::cout<<"PolyType: "<<polyType<<std::endl;

	interiorPoints.push_back(
	  std::move(std::move(GeneratePolygonPoints(polyType,
						    nPoints,
						    subBounds))));
	interiorBoundaries.insert(Shape::Polygon(interiorPoints.back()));
        for (auto i : interiorPoints.back())
          std::cout<<i<<std::endl;
      }
    }
  }

  Mesh::Mesh mesh;
  Discretization::PolygonDiscretizer polygonDiscretizer;
  polygonDiscretizer.Mesh(polygon, mesh);

  Discretization::PolygonExciser polygonExciser;
  for (auto boundary : interiorBoundaries)
    polygonExciser.ExcisePolygon(boundary, mesh);

  // for (unsigned i=0;i<100;i++)
  // {
  //   double s = Misc::Random::GetInstance().Uniform(1000)/1000.;
  //   double t = Misc::Random::GetInstance().Uniform(1000)/1000.;

  //   Shape::Point p(bounds[0]*s + bounds[1]*(1.-s),
  // 		   bounds[2]*t + bounds[3]*(1.-t));
  //   discretizer.AddInteriorPoint(p,mesh);
  // }

  Color faintRed(255,0,0,128);

  // canvas.Draw(mesh.GetPerimeter(),Red,faintRed);

  for (unsigned i=0;i<mesh.GetPerimeter().GetPoints().size();i++)
    canvas.Draw(mesh.GetPerimeter().GetPoints()[i],
  		Visualization::Color(i/(mesh.GetPerimeter()
					.GetPoints().size()-1.),
  				     Visualization::Color::BlueToRed));

  for (auto triangle : mesh.GetTriangles())
    canvas.Draw(triangle, Visualization::Black, faintRed);

  for (auto edge : mesh.GetEdges())
    if (edge.boundary)
      canvas.Draw(edge, Delaunay::Visualization::Green);

  canvas.SetTimeDelay(0.);
  canvas.Update();

  return 0;
}
