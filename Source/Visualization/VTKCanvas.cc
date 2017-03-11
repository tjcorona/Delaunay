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

#include "VTKCanvas.hh"

#include "Misc/Function.hh"

#include "Shape/Circle.hh"
#include "Shape/LineSegment.hh"
#include "Shape/Point.hh"
#include "Shape/PointUtilities.hh"
#include "Shape/Triangle.hh"
#include "Shape/Polygon.hh"
#include "Shape/ParametricCurve.hh"

#define vtkRenderingCore_AUTOINIT 2(vtkRenderingOpenGL2, vtkInteractionStyle)

#include <vtkActor.h>
#include <vtkAppendPolyData.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkCommand.h>
#include <vtkInteractorStyleImage.h>
#include <vtkLine.h>
#include <vtkNew.h>
#include <vtkPointLocator.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolygon.h>
#include <vtkPolyLine.h>
#include <vtkProperty.h>
#include <vtkRegularPolygonSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkTriangle.h>
#include <vtkTriangleFilter.h>
#include <vtkUnsignedCharArray.h>
#include <vtkVertex.h>

using namespace Delaunay::Misc;
using namespace Delaunay::Shape;

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

namespace Delaunay
{
namespace Visualization
{

class VTKCanvas::Internals
{
public:
  Internals(double dim_x_min,
	    double dim_x_max,
	    double dim_y_min,
	    double dim_y_max) :
    Polydata(vtkSmartPointer<vtkPolyData>::New()),
    Dim_x_min(dim_x_min), Dim_x_max(dim_x_max),
    Dim_y_min(dim_y_min), Dim_y_max(dim_y_max),
    delay_ms(std::numeric_limits<double>::max())
    {
      Renderer->SetBackground(1.,1.,1.);
    }

  vtkIdType AddPoint(const Shape::Point& point)
  {
    vtkIdType id;
    double p[3] = {point.x, point.y, 0.};
    this->Locator->InsertUniquePoint(p, id);
    return id;
  }

  void AddColorToArray(const Color& color, vtkUnsignedCharArray* array)
  {
    unsigned char c[4];
    c[0] = color.red;
    c[1] = color.green;
    c[2] = color.blue;
    c[3] = color.alpha;

    array->InsertNextTypedTuple(c);
  }

  void AddVertexColor(const Color& color)
    { this->AddColorToArray(color, this->VertexColors.Get()); }
  void AddLineColor(const Color& color)
    { this->AddColorToArray(color, this->LineColors.Get()); }
  void AddPolyColor(const Color& color)
    { this->AddColorToArray(color, this->PolyColors.Get()); }

  double Dim_x_min;
  double Dim_x_max;
  double Dim_y_min;
  double Dim_y_max;
  double delay_ms;

  vtkNew<vtkRenderer> Renderer;
  vtkNew<vtkPointLocator> Locator;
  vtkSmartPointer<vtkPolyData> Polydata;
  vtkNew<vtkUnsignedCharArray> VertexColors;
  vtkNew<vtkUnsignedCharArray> LineColors;
  vtkNew<vtkUnsignedCharArray> PolyColors;
};

VTKCanvas::VTKCanvas(double dim_x_min,
		     double dim_x_max,
		     double dim_y_min,
		     double dim_y_max)
{
  this->internals = new Internals(dim_x_min, dim_x_max, dim_y_min, dim_y_max);

  vtkNew<vtkPoints> points;
  vtkNew<vtkCellArray> verts;
  vtkNew<vtkCellArray> lines;
  vtkNew<vtkCellArray> polys;
  this->internals->Polydata->SetPoints(points.Get());
  this->internals->Polydata->SetVerts(verts.Get());
  this->internals->Polydata->SetLines(lines.Get());
  this->internals->Polydata->SetPolys(polys.Get());

  double bounds[6] = {dim_x_min,dim_x_max,dim_y_min,dim_y_max,-EPSILON,EPSILON};
  this->internals->Locator->InitPointInsertion(points.Get(),bounds);

  this->internals->VertexColors->SetNumberOfComponents(4);
  this->internals->LineColors->SetNumberOfComponents(4);
  this->internals->PolyColors->SetNumberOfComponents(4);
}

VTKCanvas::~VTKCanvas()
{
  delete this->internals;
}

void VTKCanvas::Draw(const Function& function, double* domain,
		     const Color& color)
{
  if (!color.alpha)
    return;
}

void VTKCanvas::Draw(const Point& point,const Color& color)
{
  vtkIdType id = this->internals->AddPoint(point);

  vtkNew<vtkVertex> vertex;
  vertex->GetPointIds()->SetId(0,id);

  this->internals->Polydata->GetVerts()->InsertNextCell(vertex.Get());

  this->internals->AddVertexColor(color);
}

void VTKCanvas::Draw(const LineSegment& lineSegment,const Color& color)
{
  vtkIdType id1 = this->internals->AddPoint(lineSegment.A);
  vtkIdType id2 = this->internals->AddPoint(lineSegment.B);

  vtkNew<vtkLine> line;
  line->GetPointIds()->SetId(0,id1);
  line->GetPointIds()->SetId(1,id2);

  this->internals->Polydata->GetLines()->InsertNextCell(line.Get());

  this->internals->AddLineColor(color);
}

void VTKCanvas::Draw(const Triangle& triangle,const Color& lineColor,
		     const Color& fillColor)
{
  Draw(triangle.AB, lineColor);
  Draw(triangle.BC, lineColor);
  Draw(triangle.AC, lineColor);

  vtkIdType id1 = this->internals->AddPoint(triangle.AB.A);
  vtkIdType id2 = this->internals->AddPoint(triangle.AB.B);
  vtkIdType id3 = this->internals->AddPoint(triangle.AC.B);

  vtkNew<vtkTriangle> tri;
  tri->GetPointIds()->SetId(0,id1);
  tri->GetPointIds()->SetId(1,id2);
  tri->GetPointIds()->SetId(2,id3);

  this->internals->Polydata->GetPolys()->InsertNextCell(tri.Get());

  this->internals->AddPolyColor(fillColor);
}

void VTKCanvas::Draw(const Polygon& polygon,const Color& lineColor,
		     const Color& fillColor)
{
  vtkNew<vtkPoints> points;
  points->SetNumberOfPoints(polygon.GetPoints().size());

  double p[3] = {0.,0.,0.};
  for (auto& pt : polygon.GetPoints())
  {
    p[0] = pt.get().x; p[1] = pt.get().x;
    points->InsertNextPoint(p);
  }

  vtkNew<vtkPolyLine> line;
  line->GetPointIds()->SetNumberOfIds(polygon.GetPoints().size());
  vtkNew<vtkPolygon> poly;
  poly->GetPointIds()->SetNumberOfIds(polygon.GetPoints().size());
  for (std::size_t i=0;i<polygon.GetPoints().size();i++)
  {
    line->GetPointIds()->SetId(i,i);
    poly->GetPointIds()->SetId(i,i);
  }

  vtkNew<vtkCellArray> lines;
  lines->SetNumberOfCells(1);
  lines->InsertNextCell(line.Get());

  vtkNew<vtkCellArray> polys;
  lines->SetNumberOfCells(1);
  lines->InsertNextCell(poly.Get());

  unsigned char c[4];
  c[0] = lineColor.red;
  c[1] = lineColor.green;
  c[2] = lineColor.blue;
  c[3] = lineColor.alpha;

  vtkNew<vtkUnsignedCharArray> colors;
  colors->SetNumberOfComponents(2);
  colors->InsertNextTypedTuple(c);

  c[0] = fillColor.red;
  c[1] = fillColor.green;
  c[2] = fillColor.blue;
  c[3] = fillColor.alpha;
  colors->InsertNextTypedTuple(c);

  vtkNew<vtkPolyData> polyData;
  polyData->SetPoints(points.Get());
  polyData->SetLines(lines.Get());
  polyData->SetPolys(polys.Get());
  polyData->GetCellData()->SetScalars(colors.Get());

  vtkNew<vtkTriangleFilter> triFilter;
  triFilter->SetInputData(polyData.Get());
  triFilter->Update();

  vtkNew<vtkAppendPolyData> append;
  append->AddInputData(this->internals->Polydata);
  append->AddInputData(triFilter->GetOutput());
  append->Update();
  this->internals->Polydata->ShallowCopy(append->GetOutput());
}

void VTKCanvas::Draw(const Circle& circle,const Color& lineColor,
		     const Color& fillColor)
{
  vtkNew<vtkRegularPolygonSource> polygonSource;

  polygonSource->SetNumberOfSides(50);
  polygonSource->SetRadius(circle.Radius);
  double center[3] = {circle.Center.x, circle.Center.y, 0.};
  polygonSource->SetCenter(center);
  polygonSource->Update();

  this->internals->AddLineColor(lineColor);
  this->internals->AddPolyColor(fillColor);

  vtkNew<vtkAppendPolyData> append;
  append->AddInputData(this->internals->Polydata);
  append->AddInputData(polygonSource->GetOutput());
  append->Update();

  this->internals->Polydata->ShallowCopy(append->GetOutput());
}

void VTKCanvas::Draw(const ParametricCurve& curve,const Color& color)
{

}

void VTKCanvas::SetTimeDelay(double d)
{
  internals->delay_ms = d*((double)1.e3);
}

void VTKCanvas::Update()
{
  vtkNew<vtkUnsignedCharArray> colors;
  colors->SetNumberOfComponents(4);
  colors->SetNumberOfTuples(this->internals->VertexColors->GetNumberOfTuples() +
                            this->internals->LineColors->GetNumberOfTuples() +
                            this->internals->PolyColors->GetNumberOfTuples());

  vtkIdType counter = 0;
  unsigned char c[4];

  for (vtkIdType i=0; i<this->internals->VertexColors->GetNumberOfTuples();i++)
  {
    this->internals->VertexColors->GetTypedTuple(i,c);
    colors->SetTypedTuple(counter++,c);
  }

  for (vtkIdType i=0; i<this->internals->LineColors->GetNumberOfTuples();i++)
  {
    this->internals->LineColors->GetTypedTuple(i,c);
    colors->SetTypedTuple(counter++,c);
  }

  for (vtkIdType i=0; i<this->internals->PolyColors->GetNumberOfTuples();i++)
  {
    this->internals->PolyColors->GetTypedTuple(i,c);
    colors->SetTypedTuple(counter++,c);
  }

  this->internals->Polydata->GetCellData()->SetScalars(colors.Get());

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputData(this->internals->Polydata);

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper.Get());
  actor->GetProperty()->SetPointSize(4);
  // give the opacity setting a little push
  actor->GetProperty()->SetOpacity(0.995);

  this->internals->Renderer->AddActor(actor.Get());

  vtkNew<vtkRenderWindow> window;
  window->PointSmoothingOn();

  window->SetSize(1000,1000);

  window->AddRenderer(this->internals->Renderer.Get());

  vtkNew<vtkInteractorStyleImage> imageStyle;
  double leftToRight[3] = {1.,0.,0.};
  double bottomToTop[3] = {0.,1.,0.};
  imageStyle->SetImageOrientation(leftToRight, bottomToTop);
  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window.Get());
  interactor->SetInteractorStyle(imageStyle.Get());

  if (this->internals->delay_ms > EPSILON)
  {
    vtkNew<vtkTimerCallback> cb;
    interactor->AddObserver(vtkCommand::TimerEvent, cb.Get());
    int timerId = interactor->CreateOneShotTimer(this->internals->delay_ms);
  }

  window->Render();
  interactor->Start();
}

}
}
