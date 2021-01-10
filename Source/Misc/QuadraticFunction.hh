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

#ifndef DELAUNAY_MISC_QUADRATICFUNCTION_HH
#define DELAUNAY_MISC_QUADRATICFUNCTION_HH

#include "Misc/Export.hh"

#include "Function.hh"

namespace Delaunay
{
namespace Misc
{

class DELAUNAYMISC_EXPORT QuadraticFunction : public Function
{
public:
  QuadraticFunction();
  double operator() (double x) const;

  void SetCoefficients(double aa, double bb, double cc);
  void SetCoefficients(double* coeff);

protected:
  double coefficients[3];
};

}
}

#endif
