#ifndef DTCOLOR_HH
#define DTCOLOR_HH

class DTColor
{
public:
  DTColor(unsigned char r,unsigned char g,unsigned char b);
  DTColor(unsigned hex);
  
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

extern const DTColor White;
extern const DTColor Black;
extern const DTColor Red;
extern const DTColor Orange;
extern const DTColor Yellow;
extern const DTColor Green;
extern const DTColor Blue;
extern const DTColor Cyan;
extern const DTColor Purple;

#endif /* DTCOLOR_HH */
