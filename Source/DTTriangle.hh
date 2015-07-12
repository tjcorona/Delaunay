#ifndef DTTRIANGLE_HH
#define DTTRIANGLE_HH

#include <set>

#include "DTPoint.hh"

class DTTriangle
{
public:
  bool Contains(const DTPoint& p) const;

  class Edge;
  typedef std::set<const DTTriangle*> TriangleSet;
  typedef std::set<const Edge*> EdgeSet;

  class Vertex : public DTPoint
  {
  public:
    Vertex(const DTPoint& p) : DTPoint(p) {}
    Vertex(double x,double y) : DTPoint(x,y) {}

    mutable TriangleSet triangles;
    mutable EdgeSet edges;
  };

  class Edge
  {
  public:
    Edge(const Vertex& p1,const Vertex& p2);
    ~Edge();

    friend bool operator==(const Edge& e1,const Edge& e2)
    {
      return (e1.A == e2.A && e1.B == e2.B);
    }

    friend bool operator!=(const Edge& e1,const Edge& e2)
    {
      return !(e1==e2);
    }

    friend bool operator<(const Edge& e1,const Edge& e2)
    {
      return (e1.A != e2.A ? e1.A < e2.A : e1.B < e2.B);
    }

    friend bool operator>=(const Edge& e1,const Edge& e2)
    {
      return !(e1<e2);
    }
  
    friend bool operator>(const Edge& e1,const Edge& e2)
    {
      return (e1.A != e2.A ? e1.A > e2.A : e1.B > e2.B);
    }

    friend bool operator<=(const Edge& e1,const Edge& e2)
    {
      return !(e1>e2);
    }
  
    mutable TriangleSet triangles;
    const Vertex& A;
    const Vertex& B;
  };

  DTTriangle(const Vertex&,const Vertex&,const Vertex&);
  ~DTTriangle();

  friend bool operator==(const DTTriangle& t1,const DTTriangle& t2)
  {
    return (t1.A == t2.A && t1.B == t2.B && t1.C == t2.C);
  }

  friend bool operator!=(const DTTriangle& t1,const DTTriangle& t2)
  {
    return !(t1==t2);
  }

  friend bool operator<(const DTTriangle& t1,const DTTriangle& t2)
  {
    return (t1.A!=t2.A ? t1.A<t2.A : (t1.B!=t2.B ? t1.B<t2.B : t1.C<t2.C));
  }

  friend bool operator>=(const DTTriangle& t1,const DTTriangle& t2)
  {
    return !(t1<t2);
  }
  
  friend bool operator>(const DTTriangle& t1,const DTTriangle& t2)
  {
    return (t1.A!=t2.A ? t1.A>t2.A : (t1.B!=t2.B ? t1.B>t2.B : t1.C>t2.C));
  }

  friend bool operator<=(const DTTriangle& t1,const DTTriangle& t2)
  {
    return !(t1>t2);
  }

  const Vertex& A;
  const Vertex& B;
  const Vertex& C;
  const Edge& AB;
  const Edge& BC;
  const Edge& AC;
  
  DTPoint circumcenter;
  double circumradius;

private:
  DTPoint ComputeCircumcenter() const;
};

#endif /* DTTRIANGLE_HH */
