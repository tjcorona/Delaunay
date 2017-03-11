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
#include "Discretization/CutEdge.hh"
#include "Discretization/InsertLineSegment.hh"

#include "Misc/Random.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include <algorithm>
#include <iostream>

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

  #ifdef INSERT
  Discretization::InsertLineSegment addEdge;
  #else
  Discretization::CutEdge addEdge;
  #endif
  std::set<const Mesh::Edge*> insertedEdges;

  {
    Shape::Point p1(9.5,5.);
    Shape::Point p2(5.,9.5);
    Shape::LineSegment l(p1,p2);

#ifdef INSERT
    insertedEdges.insert(addEdge(l,mesh));
#else
    std::set<const Mesh::Edge*> tmp(std::move(addEdge(l,mesh)));
    insertedEdges.insert(tmp.begin(), tmp.end());
#endif
  }

  {
    Shape::Point p1(5.,9.5);
    Shape::Point p2(0.5,5.);
    Shape::LineSegment l(p1,p2);

#ifdef INSERT
    insertedEdges.insert(addEdge(l,mesh));
#else
    std::set<const Mesh::Edge*> tmp(std::move(addEdge(l,mesh)));
    insertedEdges.insert(tmp.begin(), tmp.end());
#endif
  }

  {
    Shape::Point p1(0.5,5.);
    Shape::Point p2(5.,.5);
    Shape::LineSegment l(p1,p2);

#ifdef INSERT
    insertedEdges.insert(addEdge(l,mesh));
#else
    std::set<const Mesh::Edge*> tmp(std::move(addEdge(l,mesh)));
    insertedEdges.insert(tmp.begin(), tmp.end());
#endif
  }

  {
    Shape::Point p1(5.,.5);
    Shape::Point p2(9.5,5.);
    Shape::LineSegment l(p1,p2);

#ifdef INSERT
    insertedEdges.insert(addEdge(l,mesh));
#else
    std::set<const Mesh::Edge*> tmp(std::move(addEdge(l,mesh)));
    insertedEdges.insert(tmp.begin(), tmp.end());
#endif
  }

  std::cout<<"there are "<<mesh.GetTriangles().size()<<" triangles, "<<mesh.GetEdges().size()<<" edges and "<<mesh.GetVertices().size()<<" vertices."<<std::endl;

  Color faintRed(255,0,0,128);


  // canvas.Draw(mesh.GetPerimeter(),Red,faintRed);

  // for (unsigned i=0;i<mesh.GetPerimeter().GetPoints().size();i++)
  //   canvas.Draw(mesh.GetPerimeter().GetPoints()[i],
  // 		Visualization::Color(i/(mesh.GetPerimeter()
  //       				.GetPoints().size()-1.),
  // 				     Visualization::Color::BlueToRed));

  // for (auto triangle : mesh.GetTriangles())
  //   canvas.Draw(triangle, Visualization::Black, faintRed);

  int i = 0;
  for (auto& triangle : mesh.GetTriangles())
  {
    canvas.Draw(triangle, Visualization::Black,
                Visualization::Color(i++/(7.),
  				     Visualization::Color::Rainbow));
    if (i == 7)
      i = 0;
  }

  for (auto vtx : mesh.GetVertices())
    canvas.Draw(vtx, Black);

  for (auto edge : insertedEdges)
  {
    canvas.Draw(*edge, White);
    canvas.Draw(edge->A(), White);
    canvas.Draw(edge->B(), White);
  }

  // for (auto edge : mesh.GetEdges())
  //   if (edge.boundary)
  //     canvas.Draw(edge, Delaunay::Visualization::Green);

  canvas.SetTimeDelay(0.);
  canvas.Update();

  return 0;
}
