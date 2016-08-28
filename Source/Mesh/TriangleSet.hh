#ifndef DELAUNAY_MESH_TRIANGLESET_HH
#define DELAUNAY_MESH_TRIANGLESET_HH

#include <set>

namespace Delaunay
{
namespace Mesh
{

class Triangle;
typedef std::set<const Triangle*> TriangleSet;

}
}

#endif
