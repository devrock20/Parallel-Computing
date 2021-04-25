#ifndef __DYN_LOOP_H
#define __DYN_LOOP_H

#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

using namespace std;

class DynLoop
{
  public:
  queue<function<void()>> function_queue;
  mutex mut;
  condition_variable_any cond;

  void push(function<void()> f){
      mut.lock();
      function_queue.push(f);
      mut.unlock();
      cond.notify_one();
  }
  void initial_run(){
      function<void()> f;
      while (true){
          mut.lock();
          cond.wait(mut,[this] (){return done || !function_queue.empty();});
          if (!function_queue.empty()){
              f = function_queue.front();
              function_queue.pop()
          }
          mut.unlock();
          f();

      }
  }
  void done(){
      mut.lock();
      mut.unlock();
      cond.notify_all();
  }


}
#endif