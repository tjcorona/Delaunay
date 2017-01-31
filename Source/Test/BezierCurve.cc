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

#include "Shape/Point.hh"
#include "Shape/BezierCurve.hh"

#include "Misc/Random.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"

#include <iostream>

using namespace Delaunay;
using namespace Delaunay::Visualization;

int main(int /*argc*/,char** /*argv*/)
{
  // create a canvas with x span from -5 to 5, and y span from -5 to 5
  Visualization::CVCanvas canvas(0.,10.,0.,10.);

  const unsigned nPoints = 5;

  std::vector<Shape::Point> bPoints;

  for (unsigned i=0;i<nPoints;i++)
  {
    Shape::Point p(Misc::Random::GetInstance().Uniform(1000)/100.,
		   Misc::Random::GetInstance().Uniform(1000)/100.);
    canvas.Draw(p,Black);
    bPoints.push_back(p);
  }

  // create a point vector from the above-defined Bezier points
  Shape::PointVector points(bPoints.begin(),bPoints.end());

  // create a closed Bezier curve from the point vector
  Shape::BezierCurve c(points,false);

  // draw the curve
  canvas.Draw(c,Red);
  canvas.SetTimeDelay(0);
  canvas.Update();

  return 0;
}
