/******************************************************************************

  This source file is part of the Delaunay project.

  Copyright T.J. Corona

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#include "Mesh/Mesh.hh"

#include "Misc/Random.hh"
#include "Shape/LineSegmentUtilities.hh"
#include "Shape/TriangleUtilities.hh"

#include <algorithm>
#include <array>
#include <cassert>
#include <limits>
#include <unordered_set>

namespace Delaunay
{
namespace Mesh
{

// const Triangle* Mesh::FindContainingTriangle(const Shape::Point& p) const
// {
//   for (auto it = this->Triangles.begin(); it != this->Triangles.end(); ++it)
//     if (Contains(*it, p))
//       return &(*it);

//   return nullptr;
// }

// const Triangle* Mesh::FindContainingTriangle(const Shape::Point& p) const
// {
//   std::unordered_set<const Triangle*> visited;
//   const Triangle* tri = nullptr;

//   while (visited.size() < this->Triangles.size())
//   {
//     if (tri == nullptr)
//     {
//       TriangleSet::const_iterator it(this->Triangles.begin());
//       std::advance(it, Misc::Random::GetInstance().
//                    Uniform(this->Triangles.size()-1));
//       while (visited.find(&(*it)) != visited.end())
//       {
//         ++it;
//         if (it == this->Triangles.end())
//           it = this->Triangles.begin();
//       }
//       tri = &(*it);
//     }

//     if (Shape::Contains(*tri, p))
//       break;

//     visited.insert(tri);

//     double distance2 = std::numeric_limits<double>::max();
//     const Triangle* nearest = nullptr;

//     std::array<const Edge*,3> edges = {&tri->AB(), &tri->BC(), &tri->AC()};

//     for (auto e : edges)
//     {
//       for (auto t : e->triangles)
//       {
//         if (visited.find(t) == visited.end())
//         {
//           if (double d2 = Shape::DistanceSquared(Centroid(*t), p) < distance2)
//           {
//             nearest = t;
//             distance2 = d2;
//           }
//         }
//       }
//     }
//     tri = nearest;
//   }

//   return tri;
// }

const Triangle* Mesh::FindContainingTriangle(const Shape::Point& p) const
{
  std::unordered_set<const Triangle*> visited;
  const Triangle* tri = nullptr;
  {
    TriangleSet::const_iterator it(this->Triangles.begin());
    std::advance(it, Misc::Random::GetInstance().
                 Uniform(this->Triangles.size()-1));
    tri = &(*it);
  }

  while (tri != nullptr)
  {
    if (Shape::Contains(*tri, p))
      break;

    visited.insert(tri);
    double distance2 = std::numeric_limits<double>::max();
    const Triangle* nearest = nullptr;

    std::array<const Edge*,3> edges = {&tri->AB(), &tri->BC(), &tri->AC()};

    for (auto e : edges)
    {
      for (auto t : e->triangles)
      {
        if (visited.find(t) == visited.end())
        {
          double d2 = Shape::DistanceSquared(Centroid(*t), p);
          if (d2 < distance2)
          {
            nearest = t;
            distance2 = d2;
          }
        }
      }
    }
    tri = nearest;
  }

  if (tri != nullptr)
    return tri;
  else
    for (auto it = this->Triangles.begin(); it != this->Triangles.end(); ++it)
      if (Contains(*it, p))
        return &(*it);

  return nullptr;
}

}
}
