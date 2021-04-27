#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>
#include <vector>
#include <thread>
#include <queue>
#include <condition_variable>
using namespace std;

//create a queue that holds the tasks
queue<int> tasks;
std::condition_variable cv;
std::mutex cv_m;
bool done = false;

//executor which picks the task in the queue and runs it.
void executor(function<void(int j)> func)
{
  while (true)
  {
    int task;
    {
      unique_lock<mutex> lk(cv_m);
      cv.wait(lk, [&] { return !tasks.empty() || done; });
      if (done && tasks.empty())
      {
        break;
      }
      task = tasks.front();
      tasks.pop();
    }
    func(task);
  }
}

void parfor(size_t beg, size_t end, size_t increment, size_t nthreads,
            std::function<void(int)> func)
{

  //create thread pool
  vector<thread> thread_stack;
  bool sorted = false;
  //start executor parallely
  for (int t = 0; t < nthreads; t += 1)
  {
    thread_stack.push_back(thread(executor, func));
  }

  //start pushing task to queue
  for (int i = beg; i < end; i += increment)
  {
    {
      lock_guard<mutex> lock(cv_m);
      tasks.push(i);
    }
    //notify that task is available
    cv.notify_one();
  }

  // all task pushed mark main task as done and wait for threads to complete them
  done = true;
  cv.notify_all();
  for (auto &itr : thread_stack)
  {
    itr.join();
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

void staticFor(size_t beg, size_t end, size_t inc, size_t nthreads,
               std::function<void(int)> func)
{
  vector<thread> thread_stack;
  int chuck = (end - beg) / nthreads;
  // cout<< "chuck: " << chuck << endl;
  int start = beg;
  for (int t = 1; t <= nthreads; t++)
  {
    // cout << "Starting a thread with: beg: " << start << "| end: " << end << " | inc: " << inc * nthreads << endl;
    thread_stack.push_back(thread(simpleFor, start, end, inc * nthreads, func));
    start += inc;
  }
  for (auto &itr : thread_stack)
  {
    itr.join();
  }
}

template <typename TLS>
void parfor(size_t beg, size_t end, size_t increment, size_t nthreads, std::function<void(TLS &)> before,
            std::function<void(int, TLS &)> func,
            std::function<void(TLS &)> after)
{
  vector<thread> thread_stack;
  vector<TLS> threadContextStorage(nthreads);
  for (int t = 0; t < nthreads; t += 1)
  {
    before(threadContextStorage[t]);
    thread_stack.push_back(thread(simpleFor, t, end, nthreads, [&, t](int j) -> void {
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

#endif
