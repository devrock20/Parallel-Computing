#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>
#include <vector>
#include <thread>
#include <queue>
#include <condition_variable>
using namespace std;

//create a queue that holds the tasks
bool done = false;
struct Task {
    int beg, end, inc;
};
class ParallelLoop {
private:
    mutex cv_m;
    vector<thread> thread_stack;
    condition_variable cv;
    function<void(int j)> taskFunc;
    vector<Task> tasks;
    int currentTask=0;
    int nofThreads;
        //executor which picks the task in the queue and runs it.
    void executor(bool flag)
    {
      while (true)
      {
        Task task;
        {
          unique_lock<mutex> lk(cv_m);
          cv.wait(lk, [&] { return done; });
          if (done && tasks.size() == currentTask)
          {
            break;
          }
          task = tasks[currentTask];
          currentTask++;
        }
        simpleFor(task.beg, task.end, task.inc, taskFunc);
        // taskFunc(task);
      }
    }
    void simpleFor(size_t beg, size_t end, size_t inc,
                  std::function<void(int)> f)
    {
      for (size_t i = beg; i < end; i += inc)
      {
        f(i);
      }
    }
public:
    ParallelLoop(int nthreads, function<void(int j)> func) {
        taskFunc = func;
        nofThreads= nthreads;
        for (int t = 0; t < nthreads; t ++) {
            thread_stack.push_back(thread(&ParallelLoop::executor, this, true));
        }
    }
    
    void addTask(size_t beg, size_t end, size_t inc) {
        for (int t = 1, start = beg; t <= nofThreads; t++, start += inc) {
            struct Task t1{ (int)start, (int)end, (int)(inc * nofThreads)};
            tasks.push_back(t1);
        }
    }
};



// void parfor(size_t beg, size_t end, size_t increment, size_t nthreads,
//             std::function<void(int)> func)
// { 

//   //create thread pool
//   vector<thread> thread_stack;
//   bool sorted = false;
//   //start executor parallely
//   for (int t = 0; t < nthreads; t += 1)
//   {
//     thread_stack.push_back(thread(executor, func));
//   }

//   //start pushing task to queue
//   for (int i = beg; i < end; i += increment)
//   {
//     {
//       lock_guard<mutex> lock(cv_m);
//       tasks.push(i);
//     }
//     //notify that task is available
//     cv.notify_one();
//   }

//   // all task pushed mark main task as done and wait for threads to complete them
//   done = true;
//   cv.notify_all();
//   for (auto &itr : thread_stack)
//   {
//     itr.join();
//   }
// }


// void staticFor(size_t beg, size_t end, size_t inc, size_t nthreads,
//               std::function<void(int)> func)
// {
//   vector<thread> thread_stack;
//   for (int t = 1, start = beg; t <= nthreads; t++, start += inc)
//   {
//     thread_stack.push_back(thread(simpleFor, start, end, inc * nthreads, func));
//   }
//   for (auto &itr : thread_stack)
//   {
//     itr.join();
//   }
// }

// template <typename TLS>
// void parfor(size_t beg, size_t end, size_t increment, size_t nthreads, std::function<void(TLS &)> before,
//             std::function<void(int, TLS &)> func,
//             std::function<void(TLS &)> after)
// {
//   vector<thread> thread_stack;
//   vector<TLS> threadContextStorage(nthreads);
//   for (int t = 0; t < nthreads; t += 1)
//   {
//     before(threadContextStorage[t]);
//     thread_stack.push_back(thread(simpleFor, t, end, nthreads, [&, t](int j) -> void {
//       func(j, threadContextStorage[t]);
//     }));
//   }
//   for (auto &itr : thread_stack)
//   {
//     itr.join();
//   }
//   for (auto &itr : threadContextStorage)
//   {
//     after(itr);
//   }
// }

#endif
