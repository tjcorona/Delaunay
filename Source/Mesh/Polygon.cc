#include "Mesh/Polygon.hh"

#include "Shape/PointUtilities.hh"
#include "Shape/LineSegment.hh"
#include "Shape/LineSegmentUtilities.hh"

#include <algorithm>
#include <cassert>

namespace Delaunay
{
namespace Mesh
{

Polygon::Polygon(const std::vector<Shape::Point>& v) : Shape::Polygon(v)
{
}

const Vertex& Polygon::operator[](unsigned i) const
{
  assert(i < this->size());
  return static_cast<const Vertex&>(this->Points[i].get());
}

// bool Polygon::Triangulate(std::vector<Triangle*>& triangles) const
// {
  // const Polygon::Vertex *vstart,*v0,*v1,*v2,*v3,*v4;
  // unsigned n = this->Vertices.size();
  // std::vector<bool> ear(this->Vertices.size());
  // std::vector<Triangle::Vertex*> triVertices;

  // vstart = v2 = &this->Vertices.front();

  // bool hasEars = false;

  // unsigned counter = 0;

  // do
  // {
  //   triVertices.push_back(new Triangle::Vertex(v2->x,v2->y));
  //   ear[v2->Idx] = IsADiagonal(v2->Previous(),v2->Next());
  //   hasEars |= ear[v2->Idx];
  //   v2 = &v2->Next();
  //   if (++counter == n+1)
  //     break;
  // }
  // while (v2 != vstart);

  // if (n == 3)
  // {
  //   triangles.push_back(new Triangle(*triVertices[0],
  // 				       *triVertices[1],
  // 				       *triVertices[2]));
  //   return true;
  // }

  // if (!hasEars || counter != n || n < 3)
  //   return false;

  // unsigned n_last = 0;

  // while (n > 2)
  // {
  //   if (n_last == n)
  //     break;
  //   n_last = n;
  //   v2 = vstart;
  //   do
  //   {
  //     if (ear[v2->Idx])
  //     {
  // 	v3 = &v2->Next(); v4 = &v3->Next();
  // 	v1 = &v2->Previous(); v0 = &v1->Previous();

  // 	ear[v1->Idx] = IsADiagonal(*v0,*v3);
  // 	ear[v3->Idx] = IsADiagonal(*v1,*v4);

  // 	triangles.push_back(new Triangle(*triVertices[v1->Idx],
  // 					   *triVertices[v2->Idx],
  // 					   *triVertices[v3->Idx]));

  // 	v1->NextIdx = v3->Idx;
  // 	v3->PrevIdx = v1->Idx;
  // 	vstart = v3;

  // 	n--;
  // 	break;
  //     }
  //     v2 = &v2->Next();
  //   }
  //   while (v2 != vstart);
  // }

  // if (n == 2)
  //   return true;
  // else
  //   return false;
// }

}
}
