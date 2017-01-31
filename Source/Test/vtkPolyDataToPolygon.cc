#include <vtkCellArray.h>
#include <vtkIdList.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkXMLPolyDataReader.h>

#include "Shape/Polygon.hh"
#include "Shape/Point.hh"

#include "Discretization/PolygonDiscretizer.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include <algorithm>
#include <iostream>

using namespace Delaunay;
using namespace Delaunay::Visualization;

int main(int argc,char** argv)
{
  if (argc == 1)
  {
    std::cout<<"vtkPolyDataToPolygon <input.vtp>"<<std::endl;
    return 1;
  }

  vtkNew<vtkXMLPolyDataReader> reader;
  reader->SetFileName(argv[1]);
  reader->Update();

  vtkPolyData* polyData = reader->GetOutput();

  double bounds[6];
  polyData->GetBounds(bounds);
  Visualization::CVCanvas canvas(bounds[0],bounds[1],bounds[2],bounds[3]);

  std::vector<Shape::Point> vertices;
  std::vector<Shape::Polygon> polygons;

  polyData->GetLines()->InitTraversal();
  vtkNew<vtkIdList> idList;
  while(polyData->GetLines()->GetNextCell(idList.GetPointer()))
  {
    for(vtkIdType pointId = 0; pointId < idList->GetNumberOfIds(); pointId++)
    {
      double* xyz = polyData->GetPoints()->GetPoint(idList->GetId(pointId));
      vertices.push_back(Shape::Point(xyz[0],xyz[1]));
    }

    polygons.push_back(Shape::Polygon(vertices));
  }

  Color faintRed(255,0,0,128);
  Color faintBlue(0,0,255,128);

  for (auto polygon : polygons)
  {
    canvas.Draw(polygon,Red,faintRed);

    for (unsigned i=0;i<polygon.GetPoints().size();i++)
    {
      canvas.Draw(polygon.GetPoints().at(i),
    		  Visualization::Color(i/(polygon.GetPoints().size()-1.),
    				       Visualization::Color::BlueToRed));
    }

    Mesh::Mesh mesh;
    Discretization::PolygonDiscretizer discretizer;
    discretizer.Mesh(polygon, mesh);

    for (auto i = mesh.GetTriangles().begin();i!=mesh.GetTriangles().end();i++)
      canvas.Draw(*i, Visualization::Black, faintBlue);
  }

  canvas.SetTimeDelay(0.);
  canvas.Update();

  return 0;
}
