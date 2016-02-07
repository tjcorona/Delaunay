#include "DTPolygon.hh"

#include "DTTriangle.hh"

#include <algorithm>
#include <iostream>

namespace
{
  int Orientation(const DTPoint& p1, const DTPoint& p2, const DTPoint& p3)
  {
    // +1: counterclockwise
    //  0: colinear
    // -1: clockwise
    DTPoint v1 = p2 - p1;
    DTPoint v2 = p3 - p1;

    double cross = v1.x*v2.y - v1.y*v2.x;
    if (fabs(cross) < EPSILON)
      return 0;
    else
      return (cross > 0. ? 1 : -1);
  }

  bool OnLineSegment(const DTPoint& p, const DTPoint& p1, const DTPoint& p2)
  {
    // Assuming colinearity, does p fall on the line segment connecting p1 and
    // p2?
    if (fabs(p1.x - p.x) > EPSILON)
      return (p1.x < p.x && p2.x > p.x) || (p2.x < p.x && p1.x > p.x);
    else
      return (p1.y < p.y && p2.y > p.y) || (p2.y < p.y && p1.y > p.y);

  }

  bool Intersect(const DTPoint& p1, const DTPoint& p2,
		 const DTPoint& m1, const DTPoint& m2)
  {
    // Does line segment (p1,p2) intersect with line segment (m1,m2)?
    int orientation[4] = {Orientation(m1,m2,p1),Orientation(m1,m2,p2),
			  Orientation(p1,p2,m1),Orientation(p1,p2,m2)};

    if (orientation[0] == 0)
      return OnLineSegment(p1,m1,m2);
    if (orientation[1] == 0)
      return OnLineSegment(p2,m1,m2);
    if (orientation[2] == 0)
      return OnLineSegment(m1,p1,p2);
    if (orientation[3] == 0)
      return OnLineSegment(m2,p1,p2);

    return (orientation[0]*orientation[1] == -1 &&
	    orientation[2]*orientation[3] == -1);
  }

  bool IsInternal(const DTPolygon::Vertex& v,const DTPoint& w)
  {
    // Is line segment (v,w) within the angle formed at vertex v?
    const DTPolygon::Vertex& v0 = v.Previous();
    const DTPolygon::Vertex& v1 = v.Next();

    if (Orientation(v0,v,v1) >= 0)
      return Orientation(v,w,v0) == 1 && Orientation(w,v,v1) == 1;
    else
      return !(Orientation(v,w,v1) >= 0 && Orientation(w,v,v0) >= 0);
  }

  bool IsADiagonal(const DTPolygon::Vertex& v1, const DTPolygon::Vertex& v2)
  {
    // Does line segment (v1,v2) lie entirely in the polygon?

    // First check if the ray from v1 to v2 starts off within the polygon, and
    // similarly for the ray from v2 to v1
    if (!IsInternal(v1,v2) || !IsInternal(v2,v1))
      return false;

    // Now that we know (v1,v2) begins and ends within the polygon, we check
    // each edge in the polygon that does not contain v1 or v2 to ensure that
    // (v1,v2) never leaves the polygon
    const DTPolygon::Vertex* pFirst,*p1,*p2;
    pFirst = p1 = &v1;
    p2 = &p1->Next();
    do
    {
      if (p1!=&v1 && p1!=&v2 && p2!=&v1 && p2!=&v2 && Intersect(v1,v2,*p1,*p2))
    	return false;
      p1 = p2;
      p2 = &p2->Next();
    }
    while (p1 != pFirst);

    return true;
  }
};

DTPolygon::DTPolygon(const std::vector<DTPoint>& vertices) : Vertices(V)
{
  unsigned positionOfSmallest = std::distance(vertices.begin(),
					      std::min_element(vertices.begin(),
							       vertices.end()));
  unsigned size = vertices.size();
  for (unsigned i=0;i<size;i++)
  {
    Vertex v(this->V,vertices[(i+positionOfSmallest)%size]);
    v.Idx = i;
    v.NextIdx = (i+1)%size;
    v.PrevIdx = (i+size-1)%size;
    this->V.push_back(v);
  }
}

DTPolygon::DTPolygon(const DTPolygon& other) : Vertices(V)
{
  unsigned size = other.Vertices.size();
  for (unsigned i=0;i<size;i++)
  {
    Vertex v(this->V,other.Vertices[i]);
    v.Idx = i;
    v.NextIdx = (i+1)%size;
    v.PrevIdx = (i+size-1)%size;
    this->V.push_back(v);
  }
}

bool DTPolygon::Contains(const DTPoint& p) const
{
  (void)p;
  return false;
}

bool DTPolygon::Triangulate(std::vector<DTTriangle*>& triangles) const
{
  const DTPolygon::Vertex *vstart,*v0,*v1,*v2,*v3,*v4;
  unsigned n = this->Vertices.size();
  std::vector<bool> ear(this->Vertices.size());
  std::vector<DTTriangle::Vertex*> triVertices;

  vstart = v2 = &this->Vertices.front();

  bool hasEars = false;

  unsigned counter = 0;

  do
  {
    triVertices.push_back(new DTTriangle::Vertex(v2->x,v2->y));
    ear[v2->Idx] = IsADiagonal(v2->Previous(),v2->Next());
    hasEars |= ear[v2->Idx];
    v2 = &v2->Next();
    if (++counter == n+1)
      break;
  }
  while (v2 != vstart);

  if (n == 3)
  {
    triangles.push_back(new DTTriangle(*triVertices[0],
				       *triVertices[1],
				       *triVertices[2]));
    return true;
  }

  if (!hasEars || counter != n || n < 3)
    return false;

  unsigned n_last = 0;

  while (n > 2)
  {
    if (n_last == n)
      break;
    n_last = n;
    v2 = vstart;
    do
    {
      if (ear[v2->Idx])
      {
  	v3 = &v2->Next(); v4 = &v3->Next();
  	v1 = &v2->Previous(); v0 = &v1->Previous();

	ear[v1->Idx] = IsADiagonal(*v0,*v3);
	ear[v3->Idx] = IsADiagonal(*v1,*v4);

	triangles.push_back(new DTTriangle(*triVertices[v1->Idx],
					   *triVertices[v2->Idx],
					   *triVertices[v3->Idx]));

	v1->NextIdx = v3->Idx;
	v3->PrevIdx = v1->Idx;
	vstart = v3;

  	n--;
	break;
      }
      v2 = &v2->Next();
    }
    while (v2 != vstart);
  }

  if (n == 2)
    return true;
  else
    return false;
}
