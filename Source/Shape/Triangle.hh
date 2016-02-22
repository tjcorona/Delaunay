#ifndef DELAUNAY_SHAPE_TRIANGLE_HH
#define DELAUNAY_SHAPE_TRIANGLE_HH

#include <set>

#include "Point.hh"

namespace Delaunay
{
namespace Shape
{

class Triangle
{
public:
  bool Contains(const Point& p) const;

  class Edge;
  typedef std::set<const Triangle*> TriangleSet;
  typedef std::set<const Edge*> EdgeSet;

  class Vertex : public Point
  {
  public:
    Vertex(const Point& p) : Point(p) {}
    Vertex(double x,double y) : Point(x,y) {}

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

  Triangle(const Vertex&,const Vertex&,const Vertex&);
  ~Triangle();

  friend bool operator==(const Triangle& t1,const Triangle& t2)
  {
    return (t1.A == t2.A && t1.B == t2.B && t1.C == t2.C);
  }

  friend bool operator!=(const Triangle& t1,const Triangle& t2)
  {
    return !(t1==t2);
  }

  friend bool operator<(const Triangle& t1,const Triangle& t2)
  {
    return (t1.A!=t2.A ? t1.A<t2.A : (t1.B!=t2.B ? t1.B<t2.B : t1.C<t2.C));
  }

  friend bool operator>=(const Triangle& t1,const Triangle& t2)
  {
    return !(t1<t2);
  }

  friend bool operator>(const Triangle& t1,const Triangle& t2)
  {
    return (t1.A!=t2.A ? t1.A>t2.A : (t1.B!=t2.B ? t1.B>t2.B : t1.C>t2.C));
  }

  friend bool operator<=(const Triangle& t1,const Triangle& t2)
  {
    return !(t1>t2);
  }

  const Vertex& A;
  const Vertex& B;
  const Vertex& C;
  const Edge& AB;
  const Edge& BC;
  const Edge& AC;

  Point circumcenter;
  double circumradius;

private:
  Point ComputeCircumcenter() const;
};

}
}

#endif
