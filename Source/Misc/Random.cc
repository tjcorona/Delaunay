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

#include "Random.hh"

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

namespace Delaunay
{
namespace Misc
{

Random* Random::fInstance = NULL;

Random::Random()
{
  struct timeval time;
  gettimeofday(&time,NULL);
  srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

Random& Random::GetInstance()
{
  if (!fInstance)
    fInstance = new Random();

  return (*fInstance);
}

void Random::Seed(unsigned seed) const
{
  srand(seed);
}

unsigned Random::Uniform(unsigned max) const
{
  if (max == 0) return 0;
  return rand()%max;
}

}
}
