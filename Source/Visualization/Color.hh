#ifndef DELAUNAY_VISUALIZATION_COLOR_HH
#define DELAUNAY_VISUALIZATION_COLOR_HH

namespace Delaunay
{
namespace Visualization
{

class Color
{
public:
  enum Palette
{
  Rainbow,
  BlueToRed,
  Grayscale
};

  Color(unsigned char r,unsigned char g,unsigned char b, unsigned char al=255);
  Color(unsigned hex, unsigned char al=255);
  Color(double range,Palette palette=Rainbow, unsigned char al=255);

  bool operator==(const Color&) const;
  bool operator!=(const Color& other) const { return !((*this) == other); }

  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
};

extern const Color Clear;
extern const Color White;
extern const Color Black;
extern const Color Red;
extern const Color Orange;
extern const Color Yellow;
extern const Color Green;
extern const Color Blue;
extern const Color Indigo;
extern const Color Violet;
extern const Color Cyan;
extern const Color Purple;

}
}

#endif
