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
  // bool sorted = false
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

#endif
