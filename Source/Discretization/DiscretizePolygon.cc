/******************************************************************************

  This source file is part of the Delaunay project.

  Copyright T.J. Corona

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#include "DiscretizePolygon.hh"

#include "Shape/PointUtilities.hh"
#include "Shape/LineSegmentUtilities.hh"
#include "Shape/Triangle.hh"

#include <cassert>
#include <exception>
#include <limits>
#include <list>
#include <map>

namespace Delaunay
{
namespace Discretization
{
void DiscretizePolygon::operator()(const Delaunay::Shape::Polygon& polygon,
				   Delaunay::Mesh::Mesh& mesh)
{
  Shape::PointVector vec;
  const Mesh::Vertex* firstVtx = nullptr;
  const Mesh::Vertex* previousVtx = nullptr;
  for (Shape::PointVector::const_iterator it=polygon.GetPoints().begin();
       it!= polygon.GetPoints().end();++it)
  {
    const Mesh::Vertex& vtx = *(this->GetVertices(mesh).emplace(*it)).first;
    if (!firstVtx)
      firstVtx = &vtx;
    if (previousVtx)
      this->GetEdges(mesh).emplace(*previousVtx, vtx, true);
    vec.push_back(std::cref(static_cast<const Shape::Point&>(vtx)));
    previousVtx = &vtx;
  }
  this->GetEdges(mesh).emplace(*previousVtx, *firstVtx, true);
  this->GetPerimeter(mesh).SetPoints(vec);

  return this->EarCuttingMethod(mesh);
}

namespace
{
  class Ring
  {
  public:
    typedef std::list<const Mesh::Vertex*> Vertices;
    typedef Vertices::iterator VtxIt;
    typedef std::map<const Mesh::Vertex*, bool> EarMap;

    Ring(const Delaunay::Shape::Polygon& p)
    {
      for (unsigned i=0; i<p.GetPoints().size(); i++)
      {
	this->Verts.push_back(static_cast<const Mesh::Vertex*>(
				&p.GetPoints()[i].get()));
	this->Ear[this->Verts.back()] = false;
      }
    }

    unsigned size() const { return Verts.size(); }

    VtxIt first() { return this->Verts.begin(); }

    VtxIt next(VtxIt it)
    {
      VtxIt next = it;
      ++next;
      if (next == this->Verts.end())
	next = this->Verts.begin();
      return next;
    }

    VtxIt previous(VtxIt it)
    {
      VtxIt previous = it;
      if (previous == this->Verts.begin())
	previous = this->Verts.end();
      return --previous;
    }

    std::size_t index(VtxIt it)
    {
      return std::distance(this->Verts.begin(), it);
    }

    bool ear(VtxIt it)
    {
      return this->Ear[*it];
    }

    void ear(VtxIt it, bool isEar)
    {
      this->Ear[*it] = isEar;
    }

    void erase(VtxIt it)
    {
      this->Ear.erase(*it);
      this->Verts.erase(it);
    }

  private:
    Vertices Verts;
    EarMap Ear;
  };

  // Is line segment (b,w) within the angle (a,b,c)?
  bool IsInternal(const Shape::Point* a, const Shape::Point* b,
		  const Shape::Point* c, const Shape::Point* w)
  {
    if (Delaunay::Shape::Orientation(*a,*b,*c) >= 0)
      return Delaunay::Shape::Orientation(*b,*w,*a) == 1 &&
	Delaunay::Shape::Orientation(*w,*b,*c) == 1;
    else
      return !(Delaunay::Shape::Orientation(*b,*w,*c) >= 0 &&
	       Delaunay::Shape::Orientation(*w,*b,*a) >= 0);
  }

  // Does line segment (v,w) lie entirely in the polygon?
  bool IsADiagonal(Ring& r, Ring::VtxIt& v, Ring::VtxIt& w)
  {
    // First check if the ray from v to w starts off within the polygon, and
    // similarly for the ray from w to v
    if (!IsInternal(*r.previous(v),*v,*r.next(v),*w) ||
	!IsInternal(*r.previous(w),*w,*r.next(w),*v))
      return false;

    // Now that we know (v,w) begins and ends within the polygon, we check
    // each edge in the polygon that does not contain v or w to ensure that
    // (v,w) never leaves the polygon
    Ring::VtxIt pFirst, p1, p2;
    pFirst = p1 = v;
    p2 = r.next(p1);
    do
    {
      if (p1!=v && p1!=w && p2!=v && p2!=w &&
	  Delaunay::Shape::IntersectOrCoincident(
	    Delaunay::Shape::LineSegment(**v,**w),
	    Delaunay::Shape::LineSegment(**p1,**p2)))
    	return false;
      p1 = p2;
      p2 = r.next(p2);
    }
    while (p1 != pFirst);

    return true;
  }
}

void DiscretizePolygon::EarCuttingMethod(Delaunay::Mesh::Mesh& mesh)
{
  if (mesh.GetPerimeter().GetPoints().size() < 3)
    throw(std::domain_error("Too few perimeter elements"));

  Ring r(mesh.GetPerimeter());

  Ring::VtxIt vstart,v0,v1,v2,v3,v4;
  unsigned n = r.size();

  vstart = v2 = r.first();
  bool hasEars = false;

  unsigned counter = 0;

  do
  {
    v1 = r.previous(v2);
    v3 = r.next(v2);
    r.ear(v2, IsADiagonal(r,v1,v3));
    hasEars |= r.ear(v2);
    v2 = r.next(v2);
    if (++counter == n+1)
      break;
  }
  while (v2 != vstart);

  if (n == 3)
  {
    this->AddTriangleToMesh(mesh,*r.previous(v2),*v2,*r.next(v2));
  }

  if (!hasEars || counter != n || n < 3)
    return;

  unsigned n_last = 0;

  while (n > 2)
  {
    if (n_last == n)
      break;
    n_last = n;
    v2 = vstart;
    do
    {
      if (r.ear(v2))
      {
  	v3 = r.next(v2); v4 = r.next(v3);
  	v1 = r.previous(v2); v0 = r.previous(v1);

  	r.ear(v1,IsADiagonal(r,v0,v3));
  	r.ear(v3,IsADiagonal(r,v1,v4));

	this->AddTriangleToMesh(mesh,*v1,*v2,*v3);

	r.erase(v2);

	v2 = v3;
  	vstart = v3;

  	n--;
  	break;
      }
      else
      {
	v2 = r.next(v2);
      }
    }
    while (v2 != vstart);
  }
}

void DiscretizePolygon::AddTriangleToMesh(Mesh::Mesh& mesh,
					   const Mesh::Vertex* a,
					   const Mesh::Vertex* b,
					   const Mesh::Vertex* c)
{
  const Mesh::Edge& ab =*(this->GetEdges(mesh).emplace(*a,*b)).first;
  const Mesh::Edge& bc =*(this->GetEdges(mesh).emplace(*b,*c)).first;
  const Mesh::Edge& ac =*(this->GetEdges(mesh).emplace(*a,*c)).first;
  this->GetTriangles(mesh).emplace(ab,bc,ac);
}

}
}
