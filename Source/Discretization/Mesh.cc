#include "Mesh.hh"

namespace Delaunay
{
namespace Discretization
{

Mesh::~Mesh()
{
  for (TriangleSet::iterator it = this->Triangles.begin();
       it != this->Triangles.end(); ++it)
    delete *it;

  for (VertexSet::iterator it = this->Vertices.begin();
       it != this->Vertices.end(); ++it)
    delete *it;
}

}
}
