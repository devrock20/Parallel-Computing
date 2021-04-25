#ifndef __DYN_LOOP_H
#define __DYN_LOOP_H

#include <functional>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;
class DynLoop
{
public:
  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel
  void parfor1(size_t beg, size_t end, size_t inc,
               std::function<void(int)> f)
  {
    for (size_t i = beg; i < end; i += inc)
    {
      f(i);
    }
  }

  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel.
  ///
  /// Each thread that participate in the calculation will have its
  /// own TLS object.
  ///
  /// Each thread will execute function before prior to any
  /// calculation on a TLS object.
  ///
  /// Each thread will be passed the TLS object it executed before on.
  ///
  /// Once the iterations are complete, each thread will execute after
  /// on the TLS object. No two thread can execute after at the same time.
  template <typename TLS>
  void parfor(size_t beg, size_t end, size_t increment, size_t nthreads,size_t granularity,std::function<void(TLS &)> before,
              std::function<void(int, TLS &)> func,
              std::function<void(TLS &)> after)
  {
    int number_of_iterations = end/granularity;
    vector<thread> thread_stack;
    vector<TLS> threadContextStorage(nthreads);
    for (int t = 0; t < nthreads; t += 1)
    {
      before(threadContextStorage[t]);
      thread_stack.push_back(thread(&DynLoop::parfor1, this, t, number_of_iterations, nthreads, [&, t](int j) -> void {
        func(j, threadContextStorage[t]);
      }));
    }
    for (auto &itr : thread_stack)
    {
      itr.join();
    }
    for (auto &itr : threadContextStorage)
    {
      after(itr);
    }
  }
};

#endif