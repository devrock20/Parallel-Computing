#ifndef __OMPLOOP__
#define __OMPLOOP__

#include <functional>

// That does not solve the static activity, but provide the same feature so it can be used as a replacement.
// You need to add -fopenmp to CXXFLAGS and LDFLAGS for this to work.
// set number of thread by called setNbThread.
class OmpLoop
{

  int nbthread;

public:
  OmpLoop()
      : nbthread(1)
  {
  }

  void setNbThread(int t)
  {
    nbthread = t;
  }

  // template<typename TLS>
  void parfor(size_t beg1, size_t beg2,
              std::function<bool(int, int)> condt,
              std::function<void(int, int)> f)
  {
#pragma omp parallel num_threads(nbthread)
    {
      // TLS tls;
      // before(tls);

#pragma omp for schedule(static)
     for (int i = beg1, j = beg2; condt(i, j); i--,j++)
      {
        f(i,j);
      }
      // #pragma omp critical
      // after(tls);
    }
  }
};

#endif