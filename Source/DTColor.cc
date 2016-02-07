#include "DTColor.hh"

const DTColor White(255,255,255);
const DTColor Black(0,0,0);
const DTColor Red(255,0,0);
const DTColor Orange(255,128,0);
const DTColor Yellow(255,255,0);
const DTColor Green(128,255,0);
const DTColor Blue(0,0,255);
const DTColor Indigo(75,0,130);
const DTColor Violet(139,0,255);
const DTColor Cyan(0,255,255);
const DTColor Purple(128,0,255);

DTColor::DTColor(unsigned char r,
		 unsigned char g,
		 unsigned char b) : red(r), green(g), blue(b) {}

DTColor::DTColor(unsigned hex)
{
  red   = ((hex & 0xFF0000) >> 16);
  green = ((hex & 0x00FF00) >> 8);
  blue  = (hex & 0x0000FF);
}

DTColor::DTColor(double range, DTColor::Palette palette)
{
  switch(palette)
  {
  case Rainbow:
  {
    if (range == 1.)
    {
      this->red = Red.red;
      this->green = Red.green;
      this->blue = Red.blue;
      return;
    }

    static const DTColor rainbow[7] =
      {Violet,Indigo,Blue,Green,Yellow,Orange,Red};

    int floor = 0;
    int ceil = 1;

    while (range > 1./7.)
    {
      ++floor;
      ++ceil;
      range -= 1./7.;
    }

    range *= 7.;

    this->red = rainbow[floor].red*(1.-range) + rainbow[ceil].red*range;
    this->green = rainbow[floor].green*(1.-range) + rainbow[ceil].green*range;
    this->blue = rainbow[floor].blue*(1.-range) + rainbow[ceil].blue*range;
    break;
  }
  case BlueToRed:
  {
    this->red = Blue.red*(1.-range) + Red.red*range;
    this->green = Blue.green*(1.-range) + Red.green*range;
    this->blue = Blue.blue*(1.-range) + Red.blue*range;

    break;
  }
  case Grayscale:
  {
    this->red = Black.red*(1.-range) + White.red*range;
    this->green = Black.green*(1.-range) + White.green*range;
    this->blue = Black.blue*(1.-range) + White.blue*range;
    break;
  }
  }
}
