#ifndef __DYN_LOOP_H
#define __DYN_LOOP_H

#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class DynLoop
{
public:
  mutex mut;
  condition_variable_any cond;
  queue<std::vector<int>>function_queue;
  bool done = false;

  void push_function(int beg,int end){
      mut.lock();
      vector <int> vector_queue;
      for (e=beg;e<=end;e++){
          vector_queue.push_back(e);
      }
      function_queue.push(vector_queue);
      mut.unlock();
      cond.notify_one();
  }

  void is_done(){
      mut.lock();
      mut.unlock();
      cond.notify_all();
  }
   
  template <typename TLS>
  void parfor(<std::function<void(int)>>f){
    vector<int> each_iteration;
       while (true){
          mut.lock();
          cond.wait(mut,[this] (){return (done) || !function_queue.empty();});
          if (!function_queue.empty()){
              each_iteration = function_queue.front();
              function_queue.pop();
          }
          for (i=each_iteration.begin();i!each_iteration.end();i++){
              f(i)
          }
  }
  // 1000/10 = 100 * 10
  // for i to g
  // queue [ [0-100],[101-200],[201-300],[301-400])]
  // in run function f(i)
  

