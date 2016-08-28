#ifndef DELAUNAY_SHAPE_TRIANGLEUTILITIES_HH
#define DELAUNAY_SHAPE_TRIANGLEUTILITIES_HH

#include "Shape/Point.hh"

namespace Delaunay
{
namespace Shape
{
class LineSegment;
class Triangle;

// Does the point lie on the perimeter of the triangle?
bool Coincident(const Triangle& t, const Point& p);

// Does the point lie within the triangle?
bool Contains(const Triangle& t, const Point& p);

// Return the area of the triangle.
double Area(const Triangle& t);

// Return the closest point on a triangle to a point.
Point ClosestPoint(const Point& p,const Triangle& t);
inline Point ClosestPoint(const Triangle& t,const Point& p)
{
  return ClosestPoint(p,t);
}

// Return the distance between a triangle and a point.
double Distance(const Point& p,const Triangle& t);
inline double Distance(const Triangle& t,const Point& p)
{
  return Distance(p,t);
}

// Does the line segment touch the triangle?
bool Coincident(const Triangle& t, const LineSegment& l);

// Does the line segment lie within the triangle?
bool Contains(const Triangle& t, const LineSegment& l);

// Does a line segment intersect a triangle?
bool Intersect(const LineSegment& l, const Triangle& t);
inline bool Intersect(const Triangle& t, const LineSegment& l)
{
  return Intersect(l,t);
}

// Return the closest point on a triangle to a line segment.
// NOPE: the parameter order should determine the shape on which the returned point lies
Point ClosestPoint(const LineSegment& l,const Triangle& t);
inline Point ClosestPoint(const Triangle& t,const LineSegment& l)
{
  return ClosestPoint(l,t);
}

// Return the distance between a triangle and a line segment.
double Distance(const LineSegment& l,const Triangle& t);
inline double Distance(const Triangle& t,const LineSegment& l)
{
  return Distance(l,t);
}

// Does the first triangle touch the second?
bool Coincident(const Triangle& t1,const Triangle& t2);

// Is the first triangle contained within the second?
bool Contains(const Triangle& t1,const Triangle& t2);

// Do the two triangles intersect?
bool Intersect(const Triangle& t1,const Triangle& t2);

// Return the closest point on a triangle to another triangle.
Point ClosestPoint(const Triangle& t1,const Triangle& t2);

// Return the distance between two triangles.
double Distance(const Triangle& t1,const Triangle& t2);
}
}

#endif
