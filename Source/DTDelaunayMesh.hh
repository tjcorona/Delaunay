#ifndef DTDELAUNAYMESH_HH
#define DTDELAUNAYMESH_HH

#include <deque>

#include "DTPoint.hh"
#include "DTMesh.hh"

class DTDelaunayMesh : public DTMesh
{
public:
  DTDelaunayMesh() {}

  void AddPerimeterPoint(const DTPoint& p);
  void AddInteriorPoint(const DTPoint& p);

  bool TestDelaunayCondition(TriangleSet& illegalTriangles) const;

private:
  void ConstructInitialMeshFromPerimeter();
  const Triangle* FindContainingTriangle(const DTPoint& p) const;
  void SplitTriangle(const Triangle* t,const Vertex* v);
  void ExtendMesh(const Vertex* v);
  void LegalizeEdges(const Vertex* v,EdgeSet &edges);
};

#endif /* DTDELAUNAYMESH_HH */
