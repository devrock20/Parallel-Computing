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
  queue<function<float(int functionid,float a, float b,float n,float intensity)>> function_queue;
  mutex mut;
  condition_variable_any cond;
  bool done = false;

  void push(std::function<float(int functionid,float a, float b,float n,float intensity)> f){
      mut.lock();
      function_queue.push(f);
      mut.unlock();
      cond.notify_one();
  }
  void initial_run(){
      function<void()> f;
      while (true){
          mut.lock();
          cond.wait(mut,[this] (){return (done) || !function_queue.empty();});
          if (!function_queue.empty()){
              f = function_queue.front();
              function_queue.pop();
          }
          mut.unlock();
          f();

      }
  }
  void is_done(){
      mut.lock();
      done = true;
      mut.unlock();
      cond.notify_all();
  }


};
#endif