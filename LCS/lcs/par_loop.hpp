#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>
#include <vector>
#include <thread>
#include <queue>
#include <iostream>
#include <condition_variable>
using namespace std;

struct Task
{
  int i, j;
};

//create a queue that holds the tasks
queue<Task> tasks;
std::condition_variable cv;
std::mutex cv_m;
bool done = false;

//executor which picks the task in the queue and runs it.
void executor(function<void(int i, int j)> func)
{
  while (true)
  {
    Task task;
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
    func(task.i, task.j);
  }
}

void parfor(size_t beg1, size_t beg2,
            std::function<bool(int, int)> condt,
            std::function<void(int, int)> post,
            std::function<void(int, int)> func,
            size_t nthreads)
{

  //create thread pool
  vector<thread> thread_stack;
  bool sorted = false;
  //start executor parallely
  for (int t = 0; t < nthreads; t += 1)
  {
    thread_stack.push_back(thread(executor, func));
    // cout << "Started: thread: " << t << endl;
  }
  //start pushing task to queue
  for (int i = beg1, j = beg2; condt(i, j); i--,j++)
  {
    {
      lock_guard<mutex> lock(cv_m);
      tasks.push(Task{i, j});
    }
    cv.notify_one();
  }

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

#endif
