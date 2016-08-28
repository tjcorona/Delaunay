#ifndef DELAUNAY_SHAPE_POINTUTILITIES_HH
#define DELAUNAY_SHAPE_POINTUTILITIES_HH

namespace Delaunay
{
namespace Shape
{
class Point;

// Returns the orientation of three points
// +1: counterclockwise
//  0: colinear
// -1: clockwise
int Orientation(const Point& p1, const Point& p2, const Point& p3);

// Assuming both points are vector valued with initial point at the origin,
// return the dot product.
double Dot(const Point& p1,const Point& p2);

// Assuming both points are vector valued with initial point at the origin,
// return the cross product Magnitude.
double Cross(const Point& p1,const Point& p2);

// Return the square of the distance between two points.
double DistanceSquared(const Point& p1, const Point& p2);

// Return the distance between two points.
double Distance(const Point& p1, const Point& p2);
}
}

#endif
