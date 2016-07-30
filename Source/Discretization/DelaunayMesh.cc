#include "DelaunayMesh.hh"

#include <cassert>
#include <exception>
#include <iostream>
#include <limits>

namespace
{
  typedef Delaunay::Shape::Point Vector;
  typedef Delaunay::Discretization::Mesh::Vertex Vertex;

  const double EPS = 1.e-6;

  int ConvexityOfStep(const Vertex* vtx0,const Vertex* vtx1,const Vertex* vtx2)
  {
    // -1: the step breaks convexity
    // 0:  the step is linear
    // 1:  the step maintains convexity

    Vector v1 = (*vtx1) - (*vtx0);
    Vector v2 = (*vtx2) - (*vtx1);

    double unnormalizedSin = v1.x*v2.y - v2.x*v1.y;
    if (fabs(unnormalizedSin) < EPS)
      return 0;
    else if (unnormalizedSin > 0.)
      return 1;
    else
      return -1;
  }
}

namespace Delaunay
{
namespace Discretization
{

void DelaunayMesh::AddPerimeterPoint(const Point& p)
{
  Vertex* vtx = new Vertex(p);

  this->Perimeter.push_back(vtx);
  this->Vertices.insert(vtx);

//////
  if (this->Vertices.size()<3)
    return;

  if (this->Vertices.size()==3)
  {
    const Vertex* vs[3];
    VertexSet::iterator it = this->Vertices.begin();
    for (unsigned i=0;i<3;i++)
      vs[i] = *it++;

    Triangle* t = new Triangle(*vs[0],*vs[1],*vs[2]);
    this->Triangles.insert(t);
    return;
  }
  ExtendMesh(vtx);
//////
}

/*
void DelaunayMesh::ConstructConvexHulls(const Vertex* v,
					  std::vector<Perimeter>& openHulls,
					  std::vector<Perimeter>& closedHulls)
{
  std::vector<Perimeter>::reverse_iterator hull = openHulls.rbegin();
  const Vertex* v1,v2;
  for (;hull!=hulls.rend();++hull)
  {
    std::vector<Perimeter>::iterator it = (*hull).end();
    v1 = *(--it);
    v2 = *(--it);

    int convexity = ConvexityOfStep(v1,v2,v);
    if (convexity != -1)
      break;
  }

  if (hull == hulls.rend())
    {
    hulls.push_back(Perimeter(2,v2,v));
    hull = --hulls.rend();
    }

  (*hull).push_back(v);
}
*/

void DelaunayMesh::ConstructInitialMeshFromPerimeter()
{
/*
  if (GetTriangles().size() != 0)
    return;

  if (this->Perimeter.size() < 3)
    throw(std::domain_error("Too few perimeter elements"));

  Perimeter::iterator perimeterVtx = this->Perimeter.begin();

  std::vector<Perimeter> closedConvexHulls;
  std::vector<Perimeter> openConvexHulls(1);
  convexHulls[0].push_back(*(perimeterVtx++));
  convexHulls[0].push_back(*(perimeterVtx++));

  for (;perimeterVtx!=this->Perimeter.end();++perimeterVtx)
    ConstructConvexHulls(*perimeterVtx,openConvexHulls,closedConvexHulls);

  std::vector<Perimeter>::iterator hull;
  for (hull = convexHulls.begin(); hull != convexHulls.end(); ++hull)
    {
      Perimeter::iterator vtx = (*hull).begin();
      const Vertex* v1 = *(vtx++);
      const Vertex* v2 = *(vtx++);
      for (; vtx != (*hull).end(); ++vtx)
	{

	}
    }
*/
}

void DelaunayMesh::AddInteriorPoint(const Point& p)
{
  if (GetTriangles().size() == 0)
    ConstructInitialMeshFromPerimeter();

  Vertex* vtx = new Vertex(p);

  const Triangle* containingTriangle = FindContainingTriangle(*vtx);

  if (containingTriangle)
    SplitTriangle(containingTriangle,vtx);
}

const Mesh::Triangle* DelaunayMesh::FindContainingTriangle(const Point& p) const
{
  for (TriangleSet::iterator it = this->Triangles.begin();
       it != this->Triangles.end(); ++it)
    if ((*it)->Contains(p))
      return (*it);

  return NULL;
}

void DelaunayMesh::SplitTriangle(const Triangle* t, const Vertex* v)
{
  // split Triangle t into 3 new triangles using vertex v, remove t from the
  // triangle set and add the 3 new triangles to the triangle set
  const Vertex& A = t->A;
  const Vertex& B = t->B;
  const Vertex& C = t->C;
  const Vertex& D = *v;

  EdgeSet edges;
  edges.insert(&(t->AB));
  edges.insert(&(t->BC));
  edges.insert(&(t->AC));

  this->Triangles.insert(new Triangle(A,B,D));
  this->Triangles.insert(new Triangle(A,C,D));
  this->Triangles.insert(new Triangle(B,C,D));

  this->Triangles.erase(t);
  delete t;

  this->Vertices.insert(v);

  LegalizeEdges(v,edges);
}

void DelaunayMesh::ExtendMesh(const Vertex* v)
{
  // append vertex v, which is exterior to the extant mesh, to the mesh

  double minPerimeterToArea = std::numeric_limits<double>::max();
  const Edge* minEdge = NULL;

  const Vertex* vlast = NULL;
  const Vertex* vi = *(this->Vertices.begin());

  unsigned vtx_counter = 0;

  do
  {
    EdgeSet::iterator edge = vi->edges.begin();
    while ((*edge)->triangles.size()!=1 || vlast == &((*edge)->A) || vlast == &((*edge)->B))
      edge++;

    bool legal = false;
    {
      // check if a new triangle with this edge and vertex is legal
      const Vertex* va = &((*edge)->A);
      const Vertex* vb = &((*edge)->B);
      const Vertex* vopp;
      const Triangle* t = *((*edge)->triangles.begin());
      vopp = &(t->A);
      if (vopp == &((*edge)->A) || vopp == &((*edge)->B))
  	vopp = &(t->B);
      if (vopp == &((*edge)->A) || vopp == &((*edge)->B))
  	vopp = &(t->C);

      bool b1 = (v->x-vb->x)*(va->y-vb->y) - (va->x-vb->x)*(v->y-vb->y) < 0.;
      bool b2 = (vopp->x-vb->x)*(va->y-vb->y) - (va->x-vb->x)*(vopp->y-vb->y) < 0.;
      legal = (b1 != b2);
    }

    if (legal)
    {
      const Vertex& A = (*edge)->A;
      const Vertex& B = (*edge)->B;
      const Vertex& C = *v;
      double perimeter = A.Distance(B) + A.Distance(C) + B.Distance(C);
      double area = fabs(A.x*(B.y-C.y) + B.x*(C.y-A.y) + C.x*(A.y-B.y))*.5;
      double perimeterToArea = perimeter/area;

      if (perimeterToArea < minPerimeterToArea)
  	minEdge = (*edge);
    }

    vlast = vi;
    vi = (vi == &((*edge)->A) ? &((*edge)->B) : &((*edge)->A));
  }
  while (vi != *(this->Vertices.begin()));

  Triangle* t = new Triangle(*v,minEdge->A,minEdge->B);
  this->Vertices.insert(v);
  this->Triangles.insert(t);

  {
    const Vertex& A = t->A;
    const Vertex& B = t->B;
    const Vertex& C = t->C;

    EdgeSet edges;
    for (EdgeSet::iterator edge=A.edges.begin();edge!=A.edges.end();++edge)
      edges.insert(*edge);
    for (EdgeSet::iterator edge=B.edges.begin();edge!=B.edges.end();++edge)
      edges.insert(*edge);
    for (EdgeSet::iterator edge=C.edges.begin();edge!=C.edges.end();++edge)
      edges.insert(*edge);

    LegalizeEdges(v,edges);
  }

  return;
}

void DelaunayMesh::LegalizeEdges(const Vertex* v,EdgeSet &edges)
{
  if (edges.empty())
    return;

  const Edge* edge = *(edges.begin());
  edges.erase(edges.begin());

  assert(edge->triangles.size()!=0);
  assert(edge->triangles.size()<=2);

  if (edge->triangles.size() == 1)
    return LegalizeEdges(v,edges);

  const Triangle* t1 = *(edge->triangles.begin());
  const Triangle* t2 = *(++(edge->triangles.begin()));

  const Vertex* I = &(edge->A);
  const Vertex* J = &(edge->B);
  const Vertex* K = &(t1->A);
  if (K == I || K == J)
    K = &(t1->B);
  if (K == I || K == J)
    K = &(t1->C);
  const Vertex* L = &(t2->A);
  if (L == I || L == J)
    L = &(t2->B);
  if (L == I || L == J)
    L = &(t2->C);

  bool isLegal = true;

  const Point* C = &(t1->circumcenter);

  double d2 = (C->x-L->x)*(C->x-L->x) + (C->y-L->y)*(C->y-L->y);
  if (v == L)
  {
    if (d2 + EPSILON < t1->circumradius*t1->circumradius)
    {
      isLegal = false;
      for (EdgeSet::iterator edge=I->edges.begin();edge!=I->edges.end();++edge)
	if (&((*edge)->A) == K || &((*edge)->B) == K)
	  edges.insert(*edge);
      for (EdgeSet::iterator edge=J->edges.begin();edge!=J->edges.end();++edge)
	if (&((*edge)->A) == K || &((*edge)->B) == K)
	  edges.insert(*edge);
    }
  }

  if (v == K)
  {
    C = &(t2->circumcenter);
    d2 = (C->x-K->x)*(C->x-K->x) + (C->y-K->y)*(C->y-K->y);
    if (d2 + EPSILON < t2->circumradius*t2->circumradius)
    {
      isLegal = false;

      for (EdgeSet::iterator edge=I->edges.begin();edge!=I->edges.end();++edge)
	if (&((*edge)->A) == L || &((*edge)->B) == L)
	  edges.insert(*edge);
      for (EdgeSet::iterator edge=J->edges.begin();edge!=J->edges.end();++edge)
	if (&((*edge)->A) == L || &((*edge)->B) == L)
	  edges.insert(*edge);
    }
  }

  if (!isLegal)
  {
    const Triangle* new_t1 = new Triangle(*I,*L,*K);
    const Triangle* new_t2 = new Triangle(*J,*L,*K);
    this->Triangles.insert(new_t1);
    this->Triangles.insert(new_t2);
    this->Triangles.erase(t1);
    this->Triangles.erase(t2);
    delete t1;
    delete t2;
  }
  return LegalizeEdges(v,edges);
}

bool DelaunayMesh::TestDelaunayCondition(TriangleSet& illegalTriangles) const
{
  for (VertexSet::iterator vtx=this->Vertices.begin();vtx!=this->Vertices.end();++vtx)
  {
    for (Triangle::EdgeSet::iterator edge=(*vtx)->edges.begin();edge!=(*vtx)->edges.end();++edge)
    {
      // if (&(*edge)->A == (*vtx))
      {
  	if ((*edge)->triangles.size() == 1)
  	  continue;

  	const Triangle* t1 = *((*edge)->triangles.begin());
  	const Triangle* t2 = *(++((*edge)->triangles.begin()));

  	const Vertex* I = &((*edge)->A);
  	const Vertex* J = &((*edge)->B);
  	const Vertex* K = &(t1->A);
  	if (K == I || K == J)
  	  K = &(t1->B);
  	if (K == I || K == J)
  	  K = &(t1->C);
  	const Vertex* L = &(t2->A);
  	if (L == I || L == J)
  	  L = &(t2->B);
  	if (L == I || L == J)
  	  L = &(t2->C);

  	bool isLegal = true;

  	const Point* C = &(t1->circumcenter);

  	double d2 = (C->x-L->x)*(C->x-L->x) + (C->y-L->y)*(C->y-L->y);
  	if (d2 + EPSILON < t1->circumradius*t1->circumradius)
	{
	  std::cout<<d2<<" "<<t1->circumradius*t1->circumradius<<" "<<d2-t1->circumradius*t1->circumradius<<std::endl;
  	  isLegal = false;
	}

	C = &(t2->circumcenter);
	d2 = (C->x-K->x)*(C->x-K->x) + (C->y-K->y)*(C->y-K->y);
	if (d2  + EPSILON < t2->circumradius*t2->circumradius)
	{
	  std::cout<<d2<<" "<<t2->circumradius*t2->circumradius<<" "<<d2-t2->circumradius*t2->circumradius<<std::endl;
	  isLegal = false;
	}

  	if (!isLegal)
  	{
  	  illegalTriangles.insert(t1);
  	  illegalTriangles.insert(t2);
  	}
      }
    }
  }
  return illegalTriangles.empty();
}

}
}
