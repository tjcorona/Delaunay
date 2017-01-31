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

#include "Misc/Random.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include <algorithm>
#include <iostream>

using namespace Delaunay;
using namespace Delaunay::Visualization;

int main(int argc,char** argv)
{
  // create a canvas with x span from 0 to 10, and y span from 0 to 10
  double bounds[4] = {-5.,5.,-5.,5.};
  Visualization::CVCanvas canvas(bounds[0],bounds[1],bounds[2],bounds[3]);

  std::vector<Shape::Point> vertices;

  for (std::size_t i=0;i<10;i++)
  {
    vertices.push_back(Shape::Point((bounds[1]*i + bounds[0]*(10.-i))/10.,
				    bounds[2]));
  }

  for (std::size_t i=0;i<10;i++)
  {
    vertices.push_back(Shape::Point(bounds[1],
				    (bounds[3]*i + bounds[2]*(10.-i))/10.));
  }

  for (std::size_t i=0;i<10;i++)
  {
    vertices.push_back(Shape::Point((bounds[0]*i + bounds[1]*(10.-i))/10.,
				    bounds[3]));
  }

  for (std::size_t i=0;i<10;i++)
  {
    vertices.push_back(Shape::Point(bounds[0],
				    (bounds[2]*i + bounds[3]*(10.-i))/10.));
  }

  for (auto v : vertices)
    std::cout<<v<<std::endl;

  // create a polygon from the point vector
  Shape::Polygon polygon(vertices);

  Mesh::Mesh mesh;
  Discretization::DelaunayDiscretizer discretizer;
  discretizer.Mesh(polygon, mesh);

  for (unsigned i=0;i<100;i++)
  {
    double s = Misc::Random::GetInstance().Uniform(1000)/1000.;
    double t = Misc::Random::GetInstance().Uniform(1000)/1000.;

    Shape::Point p(bounds[0]*s + bounds[1]*(1.-s),
		   bounds[2]*t + bounds[3]*(1.-t));
    discretizer.AddInteriorPoint(p,mesh);
  }

  Color faintRed(255,0,0,128);

  canvas.Draw(mesh.GetPerimeter(),Red,faintRed);

  for (unsigned i=0;i<mesh.GetPerimeter().GetPoints().size();i++)
    canvas.Draw(mesh.GetPerimeter().GetPoints()[i],
  		Visualization::Color(i/(mesh.GetPerimeter()
					.GetPoints().size()-1.),
  				     Visualization::Color::BlueToRed));

  for (auto i = mesh.GetTriangles().begin(); i != mesh.GetTriangles().end(); i++)
    canvas.Draw(*i, Visualization::Black);

  canvas.SetTimeDelay(0.);
  canvas.Update();

  return 0;
}
