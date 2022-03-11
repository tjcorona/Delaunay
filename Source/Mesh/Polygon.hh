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

#ifndef DELAUNAY_MESH_POLYGON_HH
#define DELAUNAY_MESH_POLYGON_HH

#include <unordered_map>
#include <vector>

#include "Mesh/Export.hh"

#include "Mesh/Vertex.hh"
#include "Shape/Polygon.hh"

#include <cassert>

namespace Delaunay
{
namespace Mesh
{

class DELAUNAYMESH_EXPORT Polygon : public Shape::Polygon
{
public:
  class DELAUNAYMESH_EXPORT Vertices
  {
  public:
    Vertices(Shape::PointList& points) : Points(points) {}

    class DELAUNAYMESH_EXPORT const_iterator
    {
    public:
      typedef const_iterator self_type;
      typedef std::bidirectional_iterator_tag iterator_category;
      typedef const Vertex value_type;
      typedef value_type& reference;
      typedef value_type* pointer;
      typedef std::ptrdiff_t difference_type;

      const_iterator() {}
      const_iterator(const const_iterator& other) : it(other.it) {}
      const_iterator(const Shape::PointList::const_iterator& it_) : it(it_) {}
      const_iterator(Shape::PointList::const_iterator&& it_) : it(it_) {}

      ~const_iterator() {}

      operator Shape::PointList::const_iterator&() { return it; }
      operator Shape::PointList::const_iterator() const { return it; }

      const_iterator& operator=(const const_iterator& other)
        { it = other.it; return *this; }

      const_iterator& operator++() { it++; return *this; }
      const_iterator& operator--() { it--; return *this; }

      const_iterator operator++(int)
      { operator++(); return const_iterator(*this); }
      const_iterator operator--(int)
      { operator--(); return const_iterator(*this); }

      reference operator*() const { return static_cast<reference>(it->get()); }
      pointer operator->() const { return static_cast<pointer>(&(it->get())); }

      friend bool DELAUNAYMESH_EXPORT operator==(const const_iterator& it1,
                                                 const const_iterator& it2)
      { return it1.it == it2.it; }
      friend bool DELAUNAYMESH_EXPORT operator!=(const const_iterator& it1,
                                                 const const_iterator& it2)
      { return it1.it != it2.it; }

    private:
      Shape::PointList::const_iterator it;
    };

    const_iterator begin() const noexcept
    { return const_iterator(Points.begin()); }
    const_iterator end() const noexcept
    { return const_iterator(Points.end()); }

    const_iterator find(const Vertex& vertex) const
    {
      auto it = Map.find(&vertex);
      if (it != Map.end())
      {
        return const_iterator(it->second);
      }

      return const_iterator(Points.end());
    }

    std::size_t size() const noexcept { return Points.size(); }

    void assign(VertexList::const_iterator first,
                VertexList::const_iterator last)
    {
      Points.clear();

      for (; first != last; ++first)
        Points.push_back(
          std::ref(static_cast<const Shape::Point&>(first->get())));

      for (Shape::PointList::const_iterator it = Points.begin();
           it != Points.end(); ++it)
        Map[static_cast<const Vertex*>(&(it->get()))] = it;
    }

    const_iterator insert(const Vertex& vertex1, const Vertex& vertex2,
                          const Vertex& value)
    {
      Shape::PointList::const_iterator pos1 = Map[&vertex1];
      assert(*pos1 == vertex1);
      Shape::PointList::const_iterator pos2 = Map[&vertex2];
      assert(*pos2 == vertex2);
      Shape::PointList::const_iterator it =
        Points.insert((std::next(pos1) == pos2 ? pos2 : pos1), value);
      Map[&value] = it;
      return std::move(const_iterator(std::move(it)));
    }

  private:
    Shape::PointList& Points;
    std::unordered_map<const Vertex*, Shape::PointList::const_iterator> Map;
  };

  Polygon();
  Polygon(const Polygon&) = delete;
  Polygon(Polygon&&) = default;
  Polygon(const VertexList&);
  ~Polygon() {}

  void SetVertices(const VertexList& p);
  Vertices& GetVertices() { return vertices; }
  const Vertices& GetVertices() const { return vertices; }

private:
  Vertices vertices;
};

}
}

#endif
