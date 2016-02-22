#include "CVCanvas.hh"

#include "Misc/Function.hh"

#include "Shape/Circle.hh"
#include "Shape/Point.hh"
#include "Shape/Triangle.hh"
#include "Shape/Polygon.hh"
#include "Shape/ParametricCurve.hh"

using namespace Delaunay::Misc;
using namespace Delaunay::Shape;

namespace Delaunay
{
namespace Visualization
{

CVCanvas::CVCanvas(double dim_x_min,
		   double dim_x_max,
		   double dim_y_min,
		   double dim_y_max)
{
  // Create white empty image
  dim_x[0] = dim_x_min;
  dim_x[1] = dim_x_max;
  dim_y[0] = dim_y_min;
  dim_y[1] = dim_y_max;

  dim[0] = 500;
  dim[1] = 500;
  if (dim_x[1] - dim_x[0] > dim_y[1] - dim_y[0])
    dim[1] = dim[0]*(dim_y[1] - dim_y[0])/(dim_x[1] - dim_x[0]);
  else
    dim[0] = dim[1]*(dim_x[1] - dim_x[0])/(dim_y[1] - dim_y[0]);

  conversionRatio = dim[0]/(dim_x[1] - dim_x[0]);

  image = cv::Mat::zeros(dim[1],dim[0],CV_8UC3);
  image = cv::Scalar(255,255,255) - image;

  pointRadius = 4;

  delay_ms = 1.e2;
}

void CVCanvas::Draw(const Function& function, double* domain,
		    const Color& color)
{
  double dmn[2] = {(domain ? domain[0] : dim_x[0]),
                   (domain ? domain[1] : dim_x[1])};

  double x = dmn[0];
  double y = function(x);

  double min_step_x = (dim_x[1] - dim_x[0])/dim[0];
  double min_step_y = (dim_y[1] - dim_y[0])/dim[1];

  double step_x,step_y;

  uchar *tptr;
  cv::Point cvp;
  cvp.x = (dim_x[0] + x)*conversionRatio;
  cvp.y = (dim_y[1] - y)*conversionRatio;

  cv::vector<cv::Point> cvPointVec;

  while (x <= dmn[1])
  {
    if (y >= dim_y[0] && y <= dim_y[1])
      cvPointVec.push_back(cv::Point(cvp));
    else
      {
      if (!cvPointVec.empty())
      {
        const cv::Point *pts = (const cv::Point*) cv::Mat(cvPointVec).data;
        int npts = cv::Mat(cvPointVec).rows;
        cv::polylines(image,&pts, &npts, 1, false, ColorToCVScalar(color), 1,8);
        cvPointVec.clear();
      }
    }
    step_x = min_step_x;
    do
    {
      cvp.x = (x + step_x - dim_x[0])*conversionRatio;
      step_y = y;
      y = function(x);
      step_y = fabs(step_y - y);
      step_x *= .5;
    }
    while (step_y > min_step_y);

    x += 2.*step_x;
    cvp.y = (dim_y[1] - y)*conversionRatio;
  }

  if (!cvPointVec.empty())
  {
    const cv::Point *pts = (const cv::Point*) cv::Mat(cvPointVec).data;
    int npts = cv::Mat(cvPointVec).rows;
    cv::polylines(image,&pts, &npts, 1, false, ColorToCVScalar(color), 1, 8);
  }
}

void CVCanvas::Draw(const Point& point,const Color& color)
{
  cv::circle(image,PointToCVPoint(point),pointRadius,ColorToCVScalar(color),-1,8);
}

void CVCanvas::Draw(const Triangle& triangle,const Color& color)
{
  cv::line(image,PointToCVPoint(triangle.A),PointToCVPoint(triangle.B),ColorToCVScalar(color),1,8);
  cv::line(image,PointToCVPoint(triangle.B),PointToCVPoint(triangle.C),ColorToCVScalar(color),1,8);
  cv::line(image,PointToCVPoint(triangle.C),PointToCVPoint(triangle.A),ColorToCVScalar(color),1,8);
}

void CVCanvas::Draw(const Polygon& polygon,const Color& color)
{
  const unsigned size = polygon.Vertices.size();
  for (unsigned i=0;i<size;i++)
  {
    cv::line(image,PointToCVPoint(polygon.Vertices[i]),
  	     PointToCVPoint(polygon.Vertices[(i+1)%size]),
  	     ColorToCVScalar(color),1,8);
  }
}

void CVCanvas::Draw(const Circle& circle,const Color& color)
{
  cv::circle(image,PointToCVPoint(circle.Center()),
	     circle.Radius()*conversionRatio,ColorToCVScalar(color),1,8);
}

void CVCanvas::Update()
{
  cv::imshow("Image",image);
  cv::waitKey(delay_ms);
  image = cv::Mat::zeros(dim[0],dim[1],CV_8UC3);
  image = cv::Scalar(255,255,255) - image;
  cv::imshow("Image",image);
}

void CVCanvas::Draw(const ParametricCurve& curve,const Color& color)
{
  double min_step = (dim_x[1] - dim_x[0])/dim[0];

  double t = 0;

  std::vector<Point> pointVec;
  pointVec.push_back(Point(curve(0.)));

  while (t < 1.)
  {
    double step_t = .1;
    double step = 2.*min_step;
    while (step>min_step)
    {
      Point p = curve(t+step_t);
      step = p.Distance(pointVec.back());
      step_t *= .5;
    }
    t += step_t;
    pointVec.push_back(Point(curve(t)));
  }

  pointVec.push_back(Point(curve(1.)));

  cv::vector<cv::Point> cvPointVec;
  for (size_t i=0;i<pointVec.size();i++)
    cvPointVec.push_back(PointToCVPoint(pointVec.at(i)));
  const cv::Point *pts = (const cv::Point*) cv::Mat(cvPointVec).data;
  int npts = cv::Mat(cvPointVec).rows;
  cv::polylines(image,&pts, &npts, 1, false, ColorToCVScalar(color), 1, 8);
}


cv::Scalar CVCanvas::ColorToCVScalar(const Color& color)
{
  return cv::Scalar(color.blue,color.green,color.red);
}

cv::Point CVCanvas::PointToCVPoint(const Point& point)
{
  return cv::Point((point.x - dim_x[0])*conversionRatio,
        	   (dim_y[1] - point.y)*conversionRatio);
}

}
}
