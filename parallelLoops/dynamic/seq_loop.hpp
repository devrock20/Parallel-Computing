#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>
#include <vector>
#include <thread>

using namespace std;

class SeqLoop
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
  void parfor(size_t beg, size_t end, size_t increment, size_t nthreads,
              size_t granularity,
              std::function<void(TLS &)> before,
              std::function<void(int, TLS &)> func,
              std::function<void(TLS &)> after)
  {
    TLS tls;
    vector<thread> thread_stack;
    vector<TLS> localSum(nthreads);
    //
    before(tls);
    float sum = 0;

      for (size_t t = beg; t < end; t += granularity)
      {
        tls = 0;

        for (size_t j = t; j < t + granularity; ++j)
        {
          if (j == end)
          {
            break;
          }
          func(j, tls);
        }
        sum += tls;
      }
    after(sum);
    //

    // for (int t = 0; t < nthreads; t += 1)
    // {
    //   before(localSum[t]);
    //   thread_stack.push_back(thread(&SeqLoop::parfor1, this, t, end, nthreads, [&, t](int j) -> void {
    //     func(j, localSum[t]);
    //   }));
    // }
    // for (auto &itr : thread_stack)
    // {
    //   itr.join();
    // }
    // for (auto &itr : localSum)
    // {
    //   after(itr);
    // }
  }
};

#endif
