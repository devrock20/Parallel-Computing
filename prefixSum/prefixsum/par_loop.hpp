#ifndef __OMPLOOP__
#define __OMPLOOP__

#include <functional>

// That does not solve the static activity, but provide the same feature so it can be used as a replacement.
// You need to add -fopenmp to CXXFLAGS and LDFLAGS for this to work.
// set number of thread by called setNbThread.
class OmpLoop {

  int nbthread;
public:

  OmpLoop()
    :nbthread(1)
  {}
  
  void setNbThread(int t) {
    nbthread = t;
  }

  void parfor (size_t beg, size_t end, size_t increment, std::function<void(int)> f) {
	#pragma omp parallel num_threads(nbthread) {
	#pragma omp for schedule(static) 
	for (size_t i=beg; i<end; i+= increment) {
		f(i);
	}
     }
  }
};

#endif
