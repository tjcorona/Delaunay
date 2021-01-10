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

#define NANOSVG_IMPLEMENTATION
#include "Reader/nanosvg.h"

#include <iostream>

using namespace Delaunay;
using namespace Delaunay::Visualization;

int main(int argc,char** argv)
{
  if (argc == 1)
  {
    std::cout<<"ReadSVG <input.svg>"<<std::endl;
    return 1;
  }

  // Load SVG
  NSVGimage* image;
  image = nsvgParseFromFile(argv[1], "px", 96);

  // create a canvas
  Visualization::CVCanvas canvas(0.,image->width,0.,image->height);

  std::vector<std::vector<Shape::Point>> bPoints;
  std::vector<Shape::BezierCurve> bCurves;

int nShapes = 0;
  for (NSVGshape *shape = image->shapes; shape != NULL; shape = shape->next)
  {
    for (NSVGpath *path = shape->paths; path != NULL; path = path->next)
    {
      for (int i = 0; i < path->npts-1; i += 3)
      {
        float* p = &path->pts[i*2];
        bPoints.emplace_back(std::vector<Shape::Point>{
            Shape::Point(p[0], image->height - p[1]),
            Shape::Point(p[2], image->height - p[3]),
            Shape::Point(p[4], image->height - p[5]),
            Shape::Point(p[6], image->height - p[7])});
        // The definiton of "closed" is different for nanosvg than it is for
        // BezierCurve.
        bCurves.emplace_back(Shape::BezierCurve(Shape::PointList(bPoints.back().begin(), bPoints.back().end()), false /*path->closed*/));
      }
    }
  }
  nsvgDelete(image);

  // draw the curves
  int i = 0;
  for (const auto& bCurve : bCurves)
  {
    canvas.Draw(bCurve,
                Visualization::Color(i++/(bCurves.size()-1.),
                                     Visualization::Color::BlueToRed));
  }
  canvas.SetTimeDelay(0);
  canvas.Update();

  return 0;
}
