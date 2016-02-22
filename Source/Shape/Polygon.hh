#ifndef DELAUNAY_SHAPE_POLYGON_HH
#define DELAUNAY_SHAPE_POLYGON_HH

#include <vector>

#include "Point.hh"

namespace Delaunay
{
namespace Shape
{

class Triangle;

class Polygon
{
public:
  class Vertex;
  typedef std::vector<Vertex> VertexVector;

  class Vertex : public Point
  {
  public:
    friend class Polygon;

    const Vertex& Next()     const { return this->Vertices[NextIdx]; }
    const Vertex& Previous() const { return this->Vertices[PrevIdx]; }

  private:
    Vertex(VertexVector& v,const Point& p) : Point(p), Vertices(v) {}

    VertexVector& Vertices;
    unsigned Idx;
    mutable unsigned NextIdx;
    mutable unsigned PrevIdx;
  };

  Polygon(const std::vector<Point>&);
  Polygon(const Polygon&);
  ~Polygon() {}

  bool Contains(const Point& p) const;

  // Mesh Triangulate() const;

  bool Triangulate(std::vector<Triangle*>&) const;

  const VertexVector& Vertices;

private:

  VertexVector V;
};

}
}

#endif
