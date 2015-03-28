#include "DTColor.hh"

const DTColor White(255,255,255);
const DTColor Black(0,0,0);
const DTColor Red(255,0,0);
const DTColor Orange(255,128,0);
const DTColor Yellow(255,255,0);
const DTColor Green(128,255,0);
const DTColor Blue(0,0,255);
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

