#include "Random.hh"

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>

Random* Random::fInstance = NULL;

Random::Random()
{
  // srand(time(NULL));
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
