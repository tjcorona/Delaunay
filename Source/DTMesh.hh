#ifndef DTMESH_HH
#define DTMESH_HH

#include <vector>

#include "DTPoint.hh"
#include "DTTriangle.hh"

class DTMesh
{
public:
  typedef DTTriangle Triangle;
  typedef Triangle::Edge Edge;
  typedef Triangle::Vertex Vertex;
  typedef std::set<const Triangle*> TriangleSet;
  typedef std::set<const Edge*> EdgeSet;
  typedef std::set<const Vertex*> VertexSet;
  typedef std::vector<const Vertex*> Perimeter;

  virtual ~DTMesh();

  const TriangleSet& GetTriangles() const { return fTriangles; }
  const VertexSet& GetVertices() const { return fVertices; }

protected:
  Perimeter fPerimeter;
  TriangleSet fTriangles;
  VertexSet fVertices;
};

#endif /* DTMESH_HH */
