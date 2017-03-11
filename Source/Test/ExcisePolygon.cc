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

#include "Discretization/AddInteriorPoint.hh"
#include "Discretization/DiscretizePolygon.hh"
#include "Discretization/ExcisePolygon.hh"
#include "Discretization/InsertLineSegment.hh"

#include "Misc/Random.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include "PolygonGenerator.cc"

#include <algorithm>
#include <iostream>

#ifdef USE_VTK
#define vtkRenderingCore_AUTOINIT 2(vtkRenderingOpenGL2, vtkInteractionStyle)

#include <vtkCellData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkCellArray.h>
#include <vtkIdList.h>
#include <vtkLine.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkPolyLine.h>
#include <vtkTriangle.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCommand.h>
#include <vtkCallbackCommand.h>


template <typename Point, typename PointContainer>
std::size_t IndexOf(Point& point, const PointContainer& points)
{
  return std::distance(points.begin(),
		       std::find(points.begin(), points.end(), point));
}

class vtkTimerCallback : public vtkCommand
{
public:
  static vtkTimerCallback *New()
    {
      vtkTimerCallback *cb = new vtkTimerCallback;
      cb->TimerCount = 0;
      return cb;
    }

  virtual void Execute(vtkObject *caller, unsigned long eventId,
		       void *vtkNotUsed(callData))
    {
      if (vtkCommand::TimerEvent == eventId)
      {
        ++this->TimerCount;
      }

      vtkRenderWindowInteractor *iren =
	static_cast<vtkRenderWindowInteractor*>(caller);
      // Close the window
      iren->GetRenderWindow()->Finalize();

      // Stop the interactor
      iren->TerminateApp();
    }

private:
  int TimerCount;
};
#endif

void VisualizeWithVTK(const Mesh::Mesh& mesh)
{
#ifdef USE_VTK
  vtkNew<vtkPoints> points;
  points->SetNumberOfPoints(mesh.GetVertices().size());
  for (auto point : mesh.GetVertices())
  {
    points->SetPoint(IndexOf(point, mesh.GetVertices()),
		     point.x, point.y, 0.);
  }

  vtkNew<vtkUnsignedCharArray> colors;
  colors->SetName("colors");
  colors->SetNumberOfComponents(3);
  vtkIdType counter = 0;

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
  {
    float rgb[3] = {0,0,0};
    colors->InsertTuple(counter++, rgb);
  }

  vtkNew<vtkCellArray> triangles;
  for (auto i = mesh.GetTriangles().begin(); i != mesh.GetTriangles().end(); i++)
  {
    vtkNew<vtkTriangle> triangle;
    triangle->GetPointIds()->SetId(0,IndexOf(i->AB().A(),mesh.GetVertices()));
    triangle->GetPointIds()->SetId(1,IndexOf(i->AB().B(),mesh.GetVertices()));
    triangle->GetPointIds()->SetId(2,IndexOf(i->AC().B(),mesh.GetVertices()));
    triangles->InsertNextCell(triangle.GetPointer());
    {
      float rgb[3];
      rgb[0] = vtkMath::Random(0, 255);
      rgb[1] = vtkMath::Random(0, 255);
      rgb[2] = vtkMath::Random(0, 255);
      colors->InsertTuple(counter++, rgb);
    }
  }

  vtkNew<vtkPolyData> polyData;
  polyData->SetPoints(points.GetPointer());
  polyData->SetLines(lines.GetPointer());
  polyData->SetPolys(triangles.GetPointer());
  polyData->GetCellData()->SetScalars(colors.GetPointer());

  vtkNew<vtkXMLPolyDataWriter> writer;
  writer->SetInputData(polyData.GetPointer());
  writer->SetFileName("ExcisePolygon.vtp");
  writer->Write();

  // Setup the visualization pipeline
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();

  mapper->SetInputData(polyData.GetPointer());

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  // actor->GetProperty()->SetRepresentationToWireframe();
  // actor->GetProperty()->SetColor(0,0,0);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);

  vtkSmartPointer<vtkRenderWindow> window =
    vtkSmartPointer<vtkRenderWindow>::New();
  window->AddRenderer(renderer);
  window->SetSize(1000,1000);

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(window);

  renderer->AddActor(actor);
  renderer->SetBackground(1,1,1);
  renderer->ResetCamera();

  vtkSmartPointer<vtkRenderWindowInteractor> interactor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(window);

  // Sign up to receive TimerEvent
  vtkSmartPointer<vtkTimerCallback> cb =
    vtkSmartPointer<vtkTimerCallback>::New();
  interactor->AddObserver(vtkCommand::TimerEvent, cb);
  int timerId = interactor->CreateOneShotTimer(2000);

  window->Render();
  renderWindowInteractor->Start();
#else
(void)mesh;
#endif
}


using namespace Delaunay;
using namespace Delaunay::Visualization;

int main(int argc,char** argv)
{
  if (argc > 1)
  {
    int seed = atoi(argv[1]);
    std::cout<<"Random seed: "<<seed<<std::endl;
    Misc::Random::GetInstance().Seed(seed);
  }

  PolygonType testType = Random;

  // if (argc > 1)
  //   testType = static_cast<PolygonType>(atoi(argv[1]));

  // if (testType < 0 || testType > 3)
  // {
  //   std::cout<<"Polygon: possible options:"<<std::endl;
  //   std::cout<<"         0: Regular (default)"<<std::endl;
  //   std::cout<<"         1: Star Convex"<<std::endl;
  //   std::cout<<"         2: Random Evolve"<<std::endl;
  //   std::cout<<"         3: Random"<<std::endl;
  //   return 1;
  // }

  unsigned nPoints = 3 + Misc::Random::GetInstance().Uniform(20);

  // create a canvas with x span from 0 to 10, and y span from 0 to 10
  double bounds[4] = {0.,20.,0.,20.};

  double innerBounds[4] = {5,15,5,15};
  std::vector<Shape::Point> innerVertices(
    std::move(GeneratePolygonPoints(testType, nPoints, innerBounds)));

  std::vector<Shape::Point> vertices;

  unsigned nPerPts = nPoints/3 + 1;

  for (std::size_t i=0;i<nPerPts;i++)
  {
    vertices.push_back(
      Shape::Point((bounds[1]*i + bounds[0]*(nPerPts-i))/nPerPts, bounds[2]));
  }

  for (std::size_t i=0;i<nPerPts;i++)
  {
    vertices.push_back(
      Shape::Point(bounds[1], (bounds[3]*i + bounds[2]*(nPerPts-i))/nPerPts));
  }

  for (std::size_t i=0;i<nPerPts;i++)
  {
    vertices.push_back(
      Shape::Point((bounds[0]*i + bounds[1]*(nPerPts-i))/nPerPts, bounds[3]));
  }

  for (std::size_t i=0;i<nPerPts;i++)
  {
    vertices.push_back(
      Shape::Point(bounds[0], (bounds[2]*i + bounds[3]*(nPerPts-i))/nPerPts));
  }

  // create a polygon from the point vector
  Shape::Polygon polygon(vertices);

  Shape::Polygon innerPolygon(innerVertices);

  Mesh::Mesh mesh;
  Discretization::DiscretizePolygon discretize;
  discretize(polygon, mesh);

  // Discretization::AddInteriorPoint addInteriorPoint;

  // for (unsigned i=0;i<nPerPts;i++)
  // {
  //   double s = Misc::Random::GetInstance().Uniform(1000)/1000.;
  //   double t = Misc::Random::GetInstance().Uniform(1000)/1000.;

  //   Shape::Point p(bounds[0]*s + bounds[1]*(1.-s),
  // 		   bounds[2]*t + bounds[3]*(1.-t));

  //   addInteriorPoint(p,mesh);
  // }

  Discretization::ExcisePolygon excise;
  std::set<const Mesh::Edge*> insertedEdges = excise(innerPolygon, mesh);

  Color faintRed(255,0,0,128);

  if (mesh.GetTriangles().size() == 0)
  {
    std::cout<<"the polygon: "<<std::endl;
    for (unsigned i=0;i<innerVertices.size();i++)
      std::cout<<i<<": "<<innerVertices[i]<<std::endl;
  }

#ifdef USE_VTK
  VisualizeWithVTK(mesh);
#else
  Visualization::CVCanvas canvas(bounds[0],bounds[1],bounds[2],bounds[3]);

  int i = 0;
  for (auto& triangle : mesh.GetTriangles())
  {
    canvas.Draw(triangle, Visualization::Black,
                Visualization::Color(i++/(7.),
  				     Visualization::Color::Rainbow));
    if (i == 7)
      i = 0;
  }

  // for (auto vtx : mesh.GetVertices())
  //   canvas.Draw(vtx, Black);

  // for (auto edge : insertedEdges)
  //   canvas.Draw(*edge, White);

  canvas.SetTimeDelay(2.);
  canvas.Update();
#endif

  return 0;
}
