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

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"
#include "Misc/QuadraticFunction.hh"

#include <iostream>

using namespace Delaunay;
using namespace Delaunay::Visualization;

class Sin : public Misc::Function
{
public:
  Sin(double alt=1., double freq=1., double off=0.) :
    altitude(alt), frequency(freq), offset(off) {}

  double operator () (double x) const
    { return offset + altitude*sin(frequency*x); }

private:
  double altitude;
  double frequency;
  double offset;
};

int main(int /*argc*/,char** /*argv*/)
{
  // create a canvas with x span from -5 to 5, and y span from -5 to 5
  Visualization::CVCanvas canvas(-5,5,-1.5,1.5);

  // create a quadratic function
  // DTQuadraticFunction x2;

  Sin sinFunc(1.,10.,0.);

  // draw function
  // canvas.Draw(x2,NULL,Red);
  canvas.Draw(sinFunc,NULL,Red);
  canvas.SetTimeDelay(0);
  canvas.Update();

  return 0;
}
