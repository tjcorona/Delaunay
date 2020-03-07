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
#include <iostream>

using namespace Delaunay;
using namespace Delaunay::Visualization;

int main(int argc,char** argv)
{
  // create a canvas with x span from 0 to 10, and y span from 0 to 10
  double bounds[4] = {-21.,21.,-6.,6.};
  Visualization::CVCanvas canvas(bounds[0],bounds[1],bounds[2],bounds[3]);

  std::vector<Shape::Point> vertices;
  vertices.push_back(Shape::Point(-20,-5));
  vertices.push_back(Shape::Point(20,0.));
  vertices.push_back(Shape::Point(-20,5));

  // create a polygon from the point vector
  Shape::Polygon polygon(vertices);

  Mesh::Mesh mesh;
  // Discretization::DiscretizePolygon discretize;
  Discretization::ConstrainedDelaunayMesh discretize;
  discretize(polygon, mesh);

  Discretization::EnforceMinimumAngle enforceMinimumAngle;
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
    ++counter;
  }

  std::cout<<"minimum angle: "<<minimumAngle<<std::endl;

  std::cout<<"boundary size: "<<mesh.GetPerimeter().GetPoints().size()<<std::endl;

  Color faintRed(255,0,0,128);

  canvas.Draw(mesh, { Visualization::Red, Visualization::Yellow,
		      Visualization::Green, Visualization::Blue });

  for (auto triangle : mesh.GetTriangles())
    canvas.Draw(triangle, Visualization::Black);

  canvas.SetTimeDelay(0.);
  canvas.Update();

  return 0;
}
