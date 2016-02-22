#include "Mesh.hh"

namespace Delaunay
{
namespace Discretization
{

Mesh::~Mesh()
{
  for (TriangleSet::iterator it=fTriangles.begin();it!=fTriangles.end();++it)
    delete *it;

  for (VertexSet::iterator it=fVertices.begin();it!=fVertices.end();++it)
    delete *it;
}

}
}
