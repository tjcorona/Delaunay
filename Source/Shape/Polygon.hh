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

#ifndef DELAUNAY_SHAPE_POLYGON_HH
#define DELAUNAY_SHAPE_POLYGON_HH

#include <algorithm>
#include <vector>

#include "Shape/Export.hh"

#include "Shape/Point.hh"

namespace Delaunay
{
namespace Shape
{

class DELAUNAYSHAPE_EXPORT Polygon
{
public:
  template <class PointIterator>
  Polygon(const PointIterator& begin, const PointIterator& end) :
    Points(Sort(begin, end)) {}
  Polygon(const std::vector<Point>& points) :
    Points(Sort(points.begin(), points.end())) {}
  Polygon(const PointList& points) :
    Points(Sort(points.begin(), points.end())) {}
  ~Polygon() {}

  void SetPoints(const PointList& p) { this->Points = p; }
  const PointList& GetPoints() const { return this->Points; }

  friend bool operator<(const Polygon& p1,const Polygon& p2)
  {
    if (p1.Points.size() != p2.Points.size())
      return p1.Points.size() < p2.Points.size();


    PointList::const_iterator it1 = p1.Points.begin();
    PointList::const_iterator it2 = p2.Points.begin();

    for (; it1 != p1.Points.end(); ++it1, ++it2)
    {
      if (*it1 != *it2)
        return *it1 < *it2;
    }
    return false;
  }

  friend std::ostream& operator<<(std::ostream& s,const Polygon& p)
  {
    s<<"( ";
    for (PointList::const_iterator it = p.Points.begin(); it != p.Points.end(); ++it)
      s << *it << " ";
    s<<")";

    return s;
  }

protected:
  PointList::const_iterator Insert(PointList::const_iterator pos,
                                   const Point& point)
  {
    return Points.insert(pos, point);
  }

  PointList Points;

private:
  template <class Iterator>
  PointList Sort(const Iterator& begin, const Iterator& end)
  {
    PointList sortedPoints;

    Iterator smallest = std::min_element(begin, end);
    for (Iterator it = smallest; it != end; ++it)
      sortedPoints.push_back(std::cref(*it));
    for (Iterator it = begin; it != smallest; ++it)
      sortedPoints.push_back(std::cref(*it));

    return sortedPoints;
  }
};

}
}

#endif
