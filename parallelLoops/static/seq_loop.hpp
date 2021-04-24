#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>
#include <vector>
#include <thread>

using namespace std;
struct my_collection
{
  float a, b;
  int n, end, start, functionid, intensity;

  float sum;
};

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
  void parfor(size_t beg, size_t end, size_t increment, size_t nthreads, std::function<void(TLS &)> before,
              std::function<void(int, TLS &)> func,
              std::function<void(TLS &)> after)
  {
    TLS tls;
    vector<thread> thread_stack;
    // float localSum[nthreads];
    vector<TLS> localSum(nthreads);
    int flag = true;
    //0 2 4 6 8
    //1 3 5 7 9
    // beg= 0
    // nthreads = 2
    for (int t = 0; t < nthreads; t += 1)
    {
      before(localSum[t]);
      // localSum[t] = 0.0;
      // for(int j=t;j<end;j+=nthreads) {
      //   func(j, localSum[t]);
      //   // cout<< localSum[t] <<endl;
      // }
      // parfor1(t, end, nthreads, [&](int j) -> void {
      //       /**
      //        * problem here is localSum when refernce outside this lambda function is giving empty array
      //       */
      //       func(j, localSum[t]);
      //       // cout << localSum[t] << endl; //this gets printed correctly
      //     });

      auto healperFn = [&](int j) -> void {
        /**
             * problem here is localSum when refernce outside this lambda function is giving empty array
            */
        func(j, localSum[t]);
        cout << localSum[t] << " t: " << t << endl; //this gets printed correctly
      };
      thread_stack.push_back(thread(&SeqLoop::parfor1, this, t, end, nthreads, healperFn));
    }
    for (auto &itr : thread_stack)
    {
      itr.join();
    }
    for (auto &itr : localSum)
    {
      cout << itr << endl; // when accessing same array outside the lambda, I am getting 0
      after(itr);
      // tls += itr;
    }
  }
};

#endif
