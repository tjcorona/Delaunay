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

#include <vtkCellArray.h>
#include <vtkCleanPolyData.h>
#include <vtkIdList.h>
#include <vtkLine.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkPolyLine.h>
#include <vtkTriangle.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#include "Shape/Polygon.hh"
#include "Shape/Point.hh"

#include "Shape/TriangleUtilities.hh"
#include "Shape/LineSegmentUtilities.hh"
#include "Shape/PointUtilities.hh"

#include "Mesh/Mesh.hh"

#include "Misc/Random.hh"

#include "Discretization/ConstrainedDelaunayMesh.hh"
#include "Discretization/DiscretizePolygon.hh"
#include "Discretization/AddInteriorPoint.hh"
#include "Discretization/ExcisePolygon.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace Delaunay;
using namespace Delaunay::Visualization;

template <typename Point, typename PointContainer>
std::size_t IndexOf(Point& point, const PointContainer& points)
{
  return std::distance(points.begin(),
               std::find(points.begin(), points.end(), point));
}

void WriteToVTK(const Mesh::Mesh& mesh, const std::string& name)
{
  vtkNew<vtkPoints> points;
  points->SetNumberOfPoints(mesh.GetVertices().size());
  for (auto point : mesh.GetVertices())
  {
    points->SetPoint(IndexOf(point, mesh.GetVertices()),
		     point.x, point.y, 0.);
  }

  vtkNew<vtkCellArray> lines;
  vtkNew<vtkPolyLine> line;
  line->GetPointIds()->SetNumberOfIds(mesh.GetPerimeter().GetPoints().size());
  for (unsigned i=0;i<mesh.GetPerimeter().GetPoints().size();i++)
  {
    line->GetPointIds()->SetId(i,
			       IndexOf(mesh.GetPerimeter().GetPoints().at(i),
				       mesh.GetVertices()));
  }
  lines->InsertNextCell(line.GetPointer());

  vtkNew<vtkCellArray> triangles;
  for (auto i = mesh.GetTriangles().begin(); i != mesh.GetTriangles().end(); i++)
  {
    vtkNew<vtkTriangle> triangle;
    triangle->GetPointIds()->SetId(0,IndexOf(i->AB().A(),mesh.GetVertices()));
    triangle->GetPointIds()->SetId(1,IndexOf(i->AB().B(),mesh.GetVertices()));
    triangle->GetPointIds()->SetId(2,IndexOf(i->AC().B(),mesh.GetVertices()));
    triangles->InsertNextCell(triangle.GetPointer());
  }

  vtkNew<vtkPolyData> polyData;
  polyData->SetPoints(points.GetPointer());
  polyData->SetLines(lines.GetPointer());
  polyData->SetPolys(triangles.GetPointer());

  vtkNew<vtkXMLPolyDataWriter> writer;
  writer->SetInputData(polyData.GetPointer());
  writer->SetFileName(name.c_str());
  writer->Write();
}

int main(int argc,char** argv)
{
  double bounds[6];
  // std::vector<Shape::Point> vertices;
  std::vector< std::vector<Shape::Point> > vertices;

  std::string chesapeakeFile = std::string(DELAUNAY_DATA_DIR) +
    std::string("/chesapeake-0.001-100.vtp");
  // There are multiple polylines in the file; the one we are interested in is
  // the fourth
  std::size_t largestPolygon = 4;
  std::size_t nextLargestPolygon = 2;

  // Extract bounds and vertices from the VTK file
  {
    vtkNew<vtkXMLPolyDataReader> reader;
    reader->SetFileName(chesapeakeFile.c_str());
    reader->Update();

    // The input is not that friendly, so we sanitize it first
    vtkNew<vtkCleanPolyData> sanitize;
    sanitize->SetInputConnection(reader->GetOutputPort());
    // sanitize->SetTolerance(EPSILON);
    // the resolution of the image looks like 1.e-4, so we can set a pretty high
    // tolerance here with no loss in information
    sanitize->SetTolerance(2.e-5);
    sanitize->Update();

    vtkPolyData* polyData = sanitize->GetOutput();

    polyData->GetBounds(bounds);

    polyData->GetLines()->InitTraversal();
    vtkNew<vtkIdList> idList;
    std::size_t counter = 0;
    while(polyData->GetLines()->GetNextCell(idList.GetPointer()))
    {
      vertices.push_back(std::vector<Shape::Point>());
      // if (counter != nextLargestPolygon)
      // {
      //  // Skip polylines for the smaller components
      //  counter++;
      //  continue;
      // }

      // Add point ids to a vector while excising parts where the boundaries
      // touch
      std::set<vtkIdType> idSet;
      std::vector<vtkIdType> ids;

      for(vtkIdType pointId = 0; pointId < idList->GetNumberOfIds(); pointId++)
      {
    vtkIdType id = idList->GetId(pointId);
    if (idSet.find(id) != idSet.end())
    {
      while (!ids.empty() && ids.back() != id)
      {
        ids.pop_back();
      }
      // remove one more the point id is inserted exactly once
      ids.pop_back();
    }

    ids.push_back(id);
    idSet.insert(id);
      }

      for (std::size_t i=0;i<ids.size();++i)
      {
    double* xyz = polyData->GetPoints()->GetPoint(ids.at(i));

    if (vertices.back().empty() ||
        Shape::Distance(vertices.back().back(),
                Shape::Point(xyz[0],xyz[1])) > EPSILON)
      vertices.back().push_back(Shape::Point(xyz[0],xyz[1]));
      }

      if (counter == largestPolygon)
      {
    int borderDiscretization = 20;

    Shape::Point upperRightCorner(bounds[1]+.1,bounds[3]+.1);
    Shape::Point lowerRightCorner(bounds[1]+.1,bounds[2]-.1);

    for (std::size_t i=0;i<borderDiscretization;i++)
    {
      vertices.back().push_back(
        (upperRightCorner*(borderDiscretization - static_cast<double>(i)) +
         lowerRightCorner*i)/borderDiscretization);
    }

    for (std::size_t i=0;i<borderDiscretization;i++)
    {
      vertices.back().push_back(
        (lowerRightCorner*(borderDiscretization - static_cast<double>(i)) +
         vertices.back()[0]*i)/borderDiscretization);
    }
      }
      counter++;
    }
  }

  // the perimeter is backwards, so we flip it
  std::vector<Shape::Point> invertedVertices(vertices[largestPolygon].rbegin(),
                         vertices[largestPolygon].rend());

  Shape::Polygon polygon(invertedVertices);

  std::vector<Shape::Point> polygonVerticesToExcise(
    vertices[nextLargestPolygon].rbegin(),
    vertices[nextLargestPolygon].rend());
  Shape::Polygon polygonToExcise(polygonVerticesToExcise);

  Mesh::Mesh mesh;
  // Discretization::DiscretizePolygon discretize;
  Discretization::ConstrainedDelaunayMesh discretize;
  discretize(polygon, mesh);
  // discretize(polygonToExcise, mesh);

  Discretization::ExcisePolygon excise;
  std::set<const Mesh::Edge*> insertedEdges = excise(polygonToExcise, mesh);

  Discretization::AddInteriorPoint addInteriorPoint;

  WriteToVTK(mesh,"ChesapeakeBay_polygonDiscretized.vtp");

  return 0;

  std::size_t nTriangles = mesh.GetTriangles().size();
  std::size_t counter = 0;
  // Misc::Random::GetInstance().Seed(2);
  while (counter < 2000)
  {
    double s = Misc::Random::GetInstance().Uniform(1000)/1000.;
    double t = Misc::Random::GetInstance().Uniform(1000)/1000.;

    Shape::Point p(bounds[0]*s + bounds[1]*(1.-s),
           bounds[2]*t + bounds[3]*(1.-t));

    addInteriorPoint(p,mesh);
    if (nTriangles != mesh.GetTriangles().size())
    {
      nTriangles = mesh.GetTriangles().size();
      counter++;
    }
  }

  WriteToVTK(mesh,"ChesapeakeBay_pointsInserted.vtp");

  {
    Visualization::CVCanvas canvas(bounds[0]-1.,bounds[1]+1.,
                   bounds[2]-1.,bounds[3]+1.);
    canvas.SetTimeDelay(5.);

    Color faintRed(255,0,0,128);
    Color faintBlue(0,0,255,128);

    // canvas.Draw(polygon,Red,faintRed);

    for (auto triangle : mesh.GetTriangles())
      canvas.Draw(triangle, Visualization::Black, faintBlue);

    // for (unsigned i=0;i<polygon.GetPoints().size();i++)
    //   canvas.Draw(polygon.GetPoints().at(i),
    //           Visualization::Color(i/(polygon.GetPoints().size()-1.),
    //                        Visualization::Color::BlueToRed));

    for (unsigned i=0;i<vertices.size();i++)
    {
      canvas.Draw(Shape::Polygon(vertices[i]),
          Visualization::Color(i/static_cast<double>(vertices.size()),
			       Visualization::Color::Rainbow));
    }

    canvas.Update();
    nTriangles = mesh.GetTriangles().size();
  }


  return 0;
}
