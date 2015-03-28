#ifndef DTCVCANVAS_HH
#define DTCVCANVAS_HH

#include "DTCanvas.hh"

#include <opencv2/opencv.hpp>

#include "DTColor.hh"

class DTPoint;
class DTTriangle;
class DTColor;

class DTCVCanvas : public DTCanvas
{
public:
  DTCVCanvas(double dim_x_min,
	     double dim_x_max,
	     double dim_y_min,
	     double dim_y_max);
  virtual ~DTCVCanvas() {}

  void DrawPoint(const DTPoint&,const DTColor& color=Green);
  void DrawTriangle(const DTTriangle&,const DTColor& color=Green);
  void DrawCircle(const DTPoint&,double,const DTColor& color=Black);
  void Update();

private:
  cv::Scalar ColorToCVScalar(const DTColor&);
  cv::Point PointToCVPoint(const DTPoint&);
  cv::Mat image;
  unsigned pointRadius;
  double conversionRatio;
  
  double dim_x[2];
  double dim_y[2];
};

#endif /* DTCVCANVAS_HH */
