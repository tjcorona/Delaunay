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

#ifndef DELAUNAY_MISC_RANDOM_HH
#define DELAUNAY_MISC_RANDOM_HH

#include <limits>
#include <vector>

#include "Misc/Export.hh"

namespace Delaunay
{
namespace Misc
{

class DELAUNAYMISC_EXPORT Random
{
public:
  static Random& GetInstance();

  void Seed(unsigned) const;

  unsigned Uniform(unsigned max=std::numeric_limits<unsigned>::max()) const;

  template <typename Element>
  void Shuffle(std::vector<Element>& vec) const;

  private:
  Random();

  static Random* fInstance;
};

template <typename Element>
void Random::Shuffle(std::vector<Element>& vec) const
{
  if (vec.size()<=1)
    return;

  Element tmp;

  for (unsigned i=vec.size()-1;i>0;--i)
  {
    tmp = vec[i];
    unsigned index = Uniform(i);
    vec[i] = vec[index];
    vec[index] = tmp;
  }
}

}
}

#endif
