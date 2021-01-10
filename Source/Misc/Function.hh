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

#ifndef DELAUNAY_MISC_FUNCTION_HH
#define DELAUNAY_MISC_FUNCTION_HH

#include "Misc/Export.hh"

namespace Delaunay
{
namespace Misc
{

class DELAUNAYMISC_EXPORT Function
{
public:
  virtual ~Function() {}

  virtual double operator() (double x) const = 0;
};

}
}

#endif
