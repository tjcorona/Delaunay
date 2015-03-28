#include "DTCVCanvas.hh"

#include "DTPoint.hh"
#include "DTTriangle.hh"

#include <iostream>

DTCVCanvas::DTCVCanvas(double dim_x_min,
		       double dim_x_max,
		       double dim_y_min,
		       double dim_y_max)
{
  // Create white empty image
  dim_x[0] = dim_x_min;
  dim_x[1] = dim_x_max;
  dim_y[0] = dim_y_min;
  dim_y[1] = dim_y_max;

  unsigned dim[2] = {500,500};
  if (dim_x[1] - dim_x[0] > dim_y[1] - dim_y[0])
    dim[1] = dim[0]*(dim_x[1] - dim_x[0])/(dim_y[1] - dim_y[0]);
  else
    dim[0] = dim[1]*(dim_y[1] - dim_y[0])/(dim_x[1] - dim_x[0]);

  conversionRatio = dim[0]/(dim_x[1] - dim_x[0]);
  
  image = cv::Mat::zeros(dim[0],dim[1],CV_8UC3); 
  image = cv::Scalar(255,255,255) - image;

  pointRadius = 4;
}

void DTCVCanvas::DrawPoint(const DTPoint& point,const DTColor& color)
{
  cv::circle(image,PointToCVPoint(point),pointRadius,ColorToCVScalar(color),-1,8);
}

void DTCVCanvas::DrawTriangle(const DTTriangle& triangle,const DTColor& color)
{
  cv::line(image,PointToCVPoint(triangle.A),PointToCVPoint(triangle.B),ColorToCVScalar(color),1,8);
  cv::line(image,PointToCVPoint(triangle.B),PointToCVPoint(triangle.C),ColorToCVScalar(color),1,8);
  cv::line(image,PointToCVPoint(triangle.C),PointToCVPoint(triangle.A),ColorToCVScalar(color),1,8);
}

void DTCVCanvas::DrawCircle(const DTPoint& center,double radius,const DTColor& color)
{
  cv::circle(image,PointToCVPoint(center),radius*conversionRatio,ColorToCVScalar(color),1,8);
}

void DTCVCanvas::Update()
{
  cv::imshow("Image",image); 
  cv::waitKey(0);
}

cv::Scalar DTCVCanvas::ColorToCVScalar(const DTColor& color)
{
  return cv::Scalar(color.blue,color.green,color.red);
}

cv::Point DTCVCanvas::PointToCVPoint(const DTPoint& point)
{
  return cv::Point((dim_x[1] - point.x)*conversionRatio,
		   (dim_y[1] - point.y)*conversionRatio);
}

