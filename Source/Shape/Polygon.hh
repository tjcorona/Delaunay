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
  Polygon(const PointVector& points) :
    Points(Sort(points.begin(), points.end())) {}
  ~Polygon() {}

  void SetPoints(const PointVector& p) { this->Points = p; }
  const PointVector& GetPoints() const { return this->Points; }

  friend bool operator<(const Polygon& p1,const Polygon& p2)
  {
    if (p1.Points.size() != p2.Points.size())
      return p1.Points.size() < p2.Points.size();

    for (unsigned i=0;i<p1.Points.size();i++)
    {
      if (p1.Points[i] != p2.Points[i])
	return p1.Points[i] < p2.Points[i];
    }
    return false;
  }

  friend std::ostream& operator<<(std::ostream& s,const Polygon& p)
  {
    s<<"(";
    for (std::size_t i=0;i<p.Points.size();i++)
      s<<p.Points[i] << (i < p.Points.size()-1 ? "," : "");
    s<<")";

    return s;
  }

protected:
  PointVector Points;

private:
  template <class Iterator>
  PointVector Sort(const Iterator& begin, const Iterator& end)
  {
    PointVector sortedPoints;

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
