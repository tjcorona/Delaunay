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

#include "Shape/LineSegment.hh"
#include "Shape/Polygon.hh"
#include "Shape/Point.hh"

#include "Mesh/Mesh.hh"

#include "Discretization/DiscretizePolygon.hh"
#include "Discretization/SplitEdge.hh"

#include "Misc/Random.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include <algorithm>
#include <iostream>
#include <iterator>

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

  Mesh::Mesh mesh;
  Discretization::DiscretizePolygon discretize;
  discretize(polygon, mesh);

  Discretization::SplitEdge splitEdge;

  std::size_t nSplits = Misc::Random::GetInstance().Uniform(20);

  for (std::size_t i=0; i<nSplits; i++)
  {
    auto edge = mesh.GetEdges().begin();
    std::advance(edge,
		 Misc::Random::GetInstance().Uniform(mesh.GetEdges().size()-1));
    splitEdge(*edge, .3 + .4*Misc::Random::GetInstance().Uniform(1000)/1000.,
	      mesh);
  }

  std::cout<<"there are "<<mesh.GetTriangles().size()<<" triangles, "<<mesh.GetEdges().size()<<" edges and "<<mesh.GetVertices().size()<<" vertices."<<std::endl;

  // int i = 0;
  // for (auto& triangle : mesh.GetTriangles())
  // {
  //   canvas.Draw(triangle, Visualization::Black,
  //               Visualization::Color(i++/(7.),
  // 				     Visualization::Color::Rainbow));
  //   if (i == 7)
  //     i = 0;
  // }

  canvas.Draw(mesh, { Visualization::Red, Visualization::Yellow, Visualization::Green,
        Visualization::Blue });

  for (auto vtx : mesh.GetVertices())
    canvas.Draw(vtx, Black);

  canvas.SetTimeDelay(0.);
  canvas.Update();

  return 0;
}
