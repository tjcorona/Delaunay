#include "DTTriangle.hh"

DTTriangle::Edge::Edge(const Vertex& p1,const Vertex& p2) : A(p1), B(p2)
{
  A.edges.insert(this);
  B.edges.insert(this);
}

DTTriangle::Edge::~Edge()
{
  A.edges.erase(this);
  B.edges.erase(this);
}

namespace
{
  const DTTriangle::Vertex* orderedVertices[3];
  const DTTriangle::Edge* orderedEdges[3];

  const DTTriangle::Vertex& OrderTriangleElements(const DTTriangle::Vertex& p1,
						  const DTTriangle::Vertex& p2,
						  const DTTriangle::Vertex& p3)
  {
    if (p1 < p2 && p1 < p3)
    {
      orderedVertices[0] = &p1;
      if (p2 < p3)
      {
	orderedVertices[1] = &p2;
	orderedVertices[2] = &p3;
      }
      else
      {
	orderedVertices[1] = &p3;
	orderedVertices[2] = &p2;
      }
    }
    else if (p2 < p1 && p2 < p3)
    {
      orderedVertices[0] = &p2;
      if (p1 < p3)
      {
	orderedVertices[1] = &p1;
	orderedVertices[2] = &p3;
      }
      else
      {
	orderedVertices[1] = &p3;
	orderedVertices[2] = &p1;
      }
    }
    else
    {
      orderedVertices[0] = &p3;
      if (p1 < p2)
      {
	orderedVertices[1] = &p1;
	orderedVertices[2] = &p2;
      }
      else
      {
	orderedVertices[1] = &p2;
	orderedVertices[2] = &p1;
      }
    }

    typedef DTTriangle::Edge Edge;
    typedef DTTriangle::EdgeSet EdgeSet;

      // Set AB
    {
      orderedEdges[0] = NULL;
      EdgeSet& edges = orderedVertices[0]->edges;

      for (EdgeSet::iterator it=edges.begin();it!=edges.end();++it)
	  if (&((*it)->A) == orderedVertices[1] ||
	      &((*it)->B) == orderedVertices[1])
	    orderedEdges[0] = *it;
      
      if (!orderedEdges[0])
	orderedEdges[0] = new Edge(*orderedVertices[0],*orderedVertices[1]);
    }
    
    // Set BC
    {
      orderedEdges[1] = NULL;
      EdgeSet& edges = orderedVertices[1]->edges;

      for (EdgeSet::iterator it=edges.begin();it!=edges.end();++it)
	  if (&((*it)->A) == orderedVertices[2] ||
	      &((*it)->B) == orderedVertices[2])
	    orderedEdges[1] = *it;
      
      if (!orderedEdges[1])
	orderedEdges[1] = new Edge(*orderedVertices[1],*orderedVertices[2]);
    }
    
    // Set AC
    {
      orderedEdges[2] = NULL;
      EdgeSet& edges = orderedVertices[0]->edges;

      for (EdgeSet::iterator it=edges.begin();it!=edges.end();++it)
	  if (&((*it)->A) == orderedVertices[2] ||
	      &((*it)->B) == orderedVertices[2])
	    orderedEdges[2] = *it;
      
      if (!orderedEdges[2])
	orderedEdges[2] = new Edge(*orderedVertices[0],*orderedVertices[2]);
    }

    return *(orderedVertices[0]);
  }
}

DTTriangle::DTTriangle(const DTTriangle::Vertex& p1,
		       const DTTriangle::Vertex& p2,
		       const DTTriangle::Vertex& p3) :
  A(OrderTriangleElements(p1,p2,p3)),
  B(*(orderedVertices[1])),
  C(*(orderedVertices[2])),
  AB(*(orderedEdges[0])),
  BC(*(orderedEdges[1])),
  AC(*(orderedEdges[2])),
  circumcenter(ComputeCircumcenter()),
  circumradius(circumcenter.Distance(A))
{
  A.triangles.insert(this);
  B.triangles.insert(this);
  C.triangles.insert(this);
  AB.triangles.insert(this);
  BC.triangles.insert(this);
  AC.triangles.insert(this);
}

DTTriangle::~DTTriangle()
{
  A.triangles.erase(this);
  B.triangles.erase(this);
  C.triangles.erase(this);
  AB.triangles.erase(this);
  if (AB.triangles.empty()) delete &AB;
  BC.triangles.erase(this);
  if (BC.triangles.empty()) delete &BC;
  AC.triangles.erase(this);
  if (AC.triangles.empty()) delete &AC;
}

bool DTTriangle::Contains(const DTPoint& p) const
{
  double abx = B.x - A.x;
  double aby = B.y - A.y;
  double acx = C.x - A.x;
  double acy = C.y - A.y;
  double apx = p.x - A.x;
  double apy = p.y - A.y;
  
  double a = abx/aby;
  double b = acx - a*acy;
  double c = apx - a*apy;
  
  double t = c/b;
  double s = apy/aby - (acy/aby)*t;
  
  return (s >= 0.&& t >= 0. && (s + t) <= 1.);
}

DTPoint DTTriangle::ComputeCircumcenter() const
{
  double D = 2.*(A.x*(B.y - C.y) + B.x*(C.y - A.y) + C.x*(A.y - B.y));

  DTPoint U(((A.x*A.x + A.y*A.y)*(B.y-C.y) +
	     (B.x*B.x + B.y*B.y)*(C.y-A.y) +
	     (C.x*C.x + C.y*C.y)*(A.y-B.y))/D,
	    ((A.x*A.x + A.y*A.y)*(C.x-B.x) +
	     (B.x*B.x + B.y*B.y)*(A.x-C.x) +
	     (C.x*C.x + C.y*C.y)*(B.x-A.x))/D);
  return U;
}
