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
