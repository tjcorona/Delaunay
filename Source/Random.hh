#ifndef SURANDOM_HH
#define SURANDOM_HH

#include <limits>
#include <vector>

class Random
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

#endif /* SURANDOM_HH */
