#ifndef DTPOLYGON_HH
#define DTPOLYGON_HH

#include <vector>

#include "DTPoint.hh"

class DTTriangle;

class DTPolygon
{
public:
  class Vertex;
  typedef std::vector<Vertex> VertexVector;

  class Vertex : public DTPoint
  {
  public:
    friend class DTPolygon;

    const Vertex& Next()     const { return this->Vertices[NextIdx]; }
    const Vertex& Previous() const { return this->Vertices[PrevIdx]; }

  private:
    Vertex(VertexVector& v,const DTPoint& p) : DTPoint(p), Vertices(v) {}

    VertexVector& Vertices;
    unsigned Idx;
    mutable unsigned NextIdx;
    mutable unsigned PrevIdx;
  };

  DTPolygon(const std::vector<DTPoint>&);
  DTPolygon(const DTPolygon&);
  ~DTPolygon() {}

  bool Contains(const DTPoint& p) const;

  // DTMesh Triangulate() const;

  bool Triangulate(std::vector<DTTriangle*>&) const;

  const VertexVector& Vertices;

private:

  VertexVector V;
};

#endif /* DTPOLYGON_HH */
