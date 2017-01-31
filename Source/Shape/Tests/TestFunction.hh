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

#ifndef TEST_FUNCTION_HH
#define TEST_FUNCTION_HH

#include <iostream>
#include <string>

inline int test(int condition, const std::string& explanation = std::string())
{
  if (!condition)
  {
    if (!explanation.empty())
    {
      std::cerr << "## TEST FAILURE ##\n\n  " << explanation
		<< "\n\n## TEST FAILURE ##\n";
    }
    throw explanation;
  }
  return condition;
}

#endif
