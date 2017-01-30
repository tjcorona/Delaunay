#ifndef DELAUNAY_MESH_MESHER_HH
#define DELAUNAY_MESH_MESHER_HH

#include <vector>

#include "Mesh/Mesh.hh"

namespace Delaunay
{
namespace Mesh
{
class Mesher
{
public:

  virtual ~Mesher() {}

  Mesh::Polygon&       GetPerimeter(Mesh& mesh) { return mesh.Perimeter; }
  Mesh::InteriorBoundarySet& GetInteriorBoundaries(Mesh& mesh)
    { return mesh.InteriorBoundaries; }
  Mesh::VertexSet&     GetVertices(Mesh& mesh)  { return mesh.Vertices;  }
  Mesh::EdgeSet&       GetEdges(Mesh& mesh)     { return mesh.Edges;     }
  Mesh::TriangleSet&   GetTriangles(Mesh& mesh) { return mesh.Triangles; }
};

}
}

#endif
