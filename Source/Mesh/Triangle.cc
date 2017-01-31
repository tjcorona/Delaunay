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

#include "Mesh/Triangle.hh"

namespace Delaunay
{
namespace Mesh
{

Triangle::Triangle(const Edge& ab,const Edge& bc,const Edge& ac) :
  Shape::Triangle(ab,bc,ac)
  {
    this->A().triangles.insert(this);
    this->B().triangles.insert(this);
    this->C().triangles.insert(this);
    this->AB().triangles.insert(this);
    this->BC().triangles.insert(this);
    this->AC().triangles.insert(this);
  }

Triangle::~Triangle()
{
  this->A().triangles.erase(this);
  this->B().triangles.erase(this);
  this->C().triangles.erase(this);
  this->AB().triangles.erase(this);
  this->BC().triangles.erase(this);
  this->AC().triangles.erase(this);
}

const Edge& Triangle::AB() const
{
  return static_cast<const Edge&>(this->Shape::Triangle::AB);
}

const Edge& Triangle::BC() const
{
  return static_cast<const Edge&>(this->Shape::Triangle::BC);
}

const Edge& Triangle::AC() const
{
  return static_cast<const Edge&>(this->Shape::Triangle::AC);
}

const Vertex& Triangle::A() const
{
  return this->AB().A();
}

const Vertex& Triangle::B() const
{
  return this->AB().B();
}

const Vertex& Triangle::C() const
{
  return this->BC().B();
}

}
}
