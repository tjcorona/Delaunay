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
#include "Shape/LineSegmentUtilities.hh"
#include "Shape/Polygon.hh"
#include "Shape/Point.hh"
#include "Shape/TriangleUtilities.hh"

#include "Mesh/Mesh.hh"

#include "Discretization/ConstrainedDelaunayMesh.hh"
#include "Discretization/DiscretizePolygon.hh"
#include "Discretization/EnforceMinimumAngle.hh"

#include "Misc/Random.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "PolygonGenerator.cc"

using namespace Delaunay;
using namespace Delaunay::Visualization;

int main(int argc,char** argv)
{
  PolygonType testType = Random;

  if (argc > 1)
    testType = static_cast<PolygonType>(atoi(argv[1]));

  unsigned seed = -1;
  if (argc > 2)
    seed = strtoul(argv[2], NULL, 0);

  bool onlyDrawPolygon = false;
  if (argc > 3)
    onlyDrawPolygon = atoi(argv[3]) != 0;

  if (testType < 0 || testType > 3)
  {
    std::cout<<"EnforceMinimumAngle: possible options:"<<std::endl;
    std::cout<<"                     0: Regular"<<std::endl;
    std::cout<<"                     1: Star Convex"<<std::endl;
    std::cout<<"                     2: Random Evolve"<<std::endl;
    std::cout<<"                     3: Random (default)"<<std::endl;
    return 1;
  }

  if (seed == -1)
    seed = std::time(0);

  std::cout<<"random seed: "<<seed<<std::endl;
  Misc::Random::GetInstance().Seed(seed);

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

  // if (testType != Regular)
  // {
  //   std::cout<<"Currently, only regular polygons are supported."<<std::endl;
  //   return 1;
  // }

  // create a canvas with x span from 0 to 10, and y span from 0 to 10
  double bounds[4] = {0.,10.,0.,10.};
  Visualization::CVCanvas canvas(bounds[0],bounds[1],bounds[2],bounds[3]);

  // const unsigned nPoints = 3 + Misc::Random::GetInstance().Uniform(2000);
  const unsigned nPoints = 3 + Misc::Random::GetInstance().Uniform(100);

  std::cout<<"creating a polygon with "<<nPoints<<" sides"<<std::endl;

  std::vector<Shape::Point> vertices(std::move(GeneratePolygonPoints(testType,
								     nPoints,
								     bounds)));

  // create a polygon from the point vector
  Shape::Polygon polygon(vertices);

  Mesh::Mesh mesh;
  // Discretization::DiscretizePolygon discretize;
  Discretization::ConstrainedDelaunayMesh discretize;
  discretize(polygon, mesh);

  Discretization::EnforceMinimumAngle enforceMinimumAngle;
  if (!onlyDrawPolygon)
    enforceMinimumAngle(20.7, mesh);

  std::cout<<"there are "<<mesh.GetTriangles().size()<<" triangles, "<<mesh.GetEdges().size()<<" edges and "<<mesh.GetVertices().size()<<" vertices."<<std::endl;

  std::size_t counter = 0;
  double minimumAngle = 180.;
  for (auto triangle : mesh.GetTriangles())
  {
    std::array<double, 3> angles = Shape::Angles(triangle);
    for (std::size_t i = 0; i < 3; i++)
      if (minimumAngle > angles[i] * 180. / M_PI)
        minimumAngle = angles[i] * 180. / M_PI;
    // std::cout<<"angles for triangle "<<counter<<": "<<angles[0]*180./M_PI<<", "<<angles[1]*180./M_PI<<", "<<angles[2]*180./M_PI<<std::endl;
    ++counter;
  }

  std::cout<<"minimum angle: "<<minimumAngle<<std::endl;

  std::cout<<"boundary size: "<<mesh.GetPerimeter().GetPoints().size()<<std::endl;

  Color faintRed(255,0,0,128);

  // canvas.Draw(mesh.GetPerimeter(),Red,faintRed);
  canvas.Draw(mesh, { Visualization::Red, Visualization::Yellow,
		      Visualization::Green, Visualization::Blue });


  // canvas.Draw(mesh, {Red, Blue, Green, Yellow});

  for (auto triangle : mesh.GetTriangles())
    canvas.Draw(triangle, Visualization::Black);

  // for (auto edge : mesh.GetEdges())
  //   if (enforceMinimumAngle.IsEncroached(edge))
  //     canvas.Draw(edge, Delaunay::Visualization::Yellow);

  // for (unsigned i=0;i<mesh.GetPerimeter().GetPoints().size();i++)
  //   canvas.Draw(mesh.GetPerimeter().GetPoints()[i],
  // 		Visualization::Color(i/(mesh.GetPerimeter()
  // 					.GetPoints().size()-1.),
  // 				     Visualization::Color::BlueToRed));

  canvas.SetTimeDelay(0.);
  canvas.Update();

  return 0;
}
