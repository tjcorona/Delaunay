#ifndef DELAUNAY_MESH_EDGESET_HH
#define DELAUNAY_MESH_EDGESET_HH

#include <set>

namespace Delaunay
{
namespace Mesh
{

class Edge;
typedef std::set<const Edge*> EdgeSet;

}
}

#endif
