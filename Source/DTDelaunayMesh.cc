#include "DTDelaunayMesh.hh"

#include <cassert>
#include <limits>
#include <iostream>

void DTDelaunayMesh::AddPerimeterPoint(const DTPoint& p)
{
  Vertex* vtx = new Vertex(p);

  fPerimeter.push_back(vtx);
  fVertices.insert(vtx);

  if (fVertices.size()<3)
    return;

  if (fVertices.size()==3)
  {
    const Vertex* vs[3];
    VertexSet::iterator it = fVertices.begin();
    for (unsigned i=0;i<3;i++)
      vs[i] = *it++;
    
    Triangle* t = new Triangle(*vs[0],*vs[1],*vs[2]);
    fTriangles.insert(t);
    return;
  }
  ExtendMesh(vtx);
}

void DTDelaunayMesh::AddInteriorPoint(const DTPoint& p)
{
  Vertex* vtx = new Vertex(p);

  const Triangle* containingTriangle = FindContainingTriangle(*vtx);

  if (containingTriangle)
    SplitTriangle(containingTriangle,vtx);
}

const DTMesh::Triangle* DTDelaunayMesh::FindContainingTriangle(const DTPoint& p) const
{
  for (TriangleSet::iterator it=fTriangles.begin();it!=fTriangles.end();++it)
    if ((*it)->Contains(p))
      return (*it);
  return NULL;
}

void DTDelaunayMesh::SplitTriangle(const Triangle* t, const Vertex* v)
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

  fTriangles.insert(new Triangle(A,B,D));
  fTriangles.insert(new Triangle(A,C,D));
  fTriangles.insert(new Triangle(B,C,D));

  fTriangles.erase(t);
  delete t;

  fVertices.insert(v);

  LegalizeEdges(v,edges);
}

void DTDelaunayMesh::ExtendMesh(const Vertex* v)
{
  // append vertex v, which is exterior to the extant mesh, to the mesh

  double minPerimeterToArea = std::numeric_limits<double>::max();
  const Edge* minEdge = NULL;
  
  const Vertex* vlast = NULL;
  const Vertex* vi = *(fVertices.begin());

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
  while (vi != *(fVertices.begin()));

  Triangle* t = new Triangle(*v,minEdge->A,minEdge->B);
  fVertices.insert(v);
  fTriangles.insert(t);

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

void DTDelaunayMesh::LegalizeEdges(const Vertex* v,EdgeSet &edges)
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

  const DTPoint* C = &(t1->circumcenter);

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
    fTriangles.insert(new_t1);
    fTriangles.insert(new_t2);
    fTriangles.erase(t1);
    fTriangles.erase(t2);
    delete t1;
    delete t2;
  }
  return LegalizeEdges(v,edges);
}

bool DTDelaunayMesh::TestDelaunayCondition(TriangleSet& illegalTriangles) const
{
  for (VertexSet::iterator vtx=fVertices.begin();vtx!=fVertices.end();++vtx)
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

  	const DTPoint* C = &(t1->circumcenter);

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
