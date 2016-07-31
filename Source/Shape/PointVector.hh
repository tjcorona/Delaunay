#ifndef DELAUNAY_SHAPE_POINTVECTOR_HH
#define DELAUNAY_SHAPE_POINTVECTOR_HH

#include <functional>
#include <vector>

namespace Delaunay
{
namespace Shape
{

class Point;
typedef std::vector<std::reference_wrapper<const Point> > PointVector;

}
}

#endif
