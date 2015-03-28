#ifndef DTCANVAS_HH
#define DTCANVAS_HH

class DTPoint;
class DTTriangle;
class DTColor;

class DTCanvas
{
public:
  virtual ~DTCanvas() {}

  virtual void DrawPoint(const DTPoint&,const DTColor&) = 0;
  virtual void DrawTriangle(const DTTriangle&,const DTColor&) = 0;
  virtual void DrawCircle(const DTPoint&,double,const DTColor&) = 0;
  virtual void Update() {}
};

#endif /* DTCANVAS_HH */
