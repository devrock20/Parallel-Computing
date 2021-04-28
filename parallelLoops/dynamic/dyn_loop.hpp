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
        mutex cv_m;
        condition_variable cv;
        queue<int> tasks;
       bool done=false; 
    public:
    void parfor1(size_t beg, size_t end, size_t inc,
               std::function<void(int)> f)
    {
      for (size_t i = beg; i < end; i += inc)
    {
      f(i);
      //cout << "  i value" << i << endl;
    }
    }

  void push_items(int i){
    unique_lock<mutex> lk(cv_m);
    tasks.push(i);
    cv_m.unlock();
    cv.notify_one();

  }

  void all_done(){
    unique_lock<mutex> lk(cv_m);
    done = true;
    cv_m.unlock();
    cv.notify_all();
  }
  

  void executor(size_t chunk_size,
               std::function<void(int)> f){
      while(true)
      {   
          int task;
            {
                unique_lock<mutex> lk(cv_m);
                cv.wait(lk, [&] { return !tasks.empty() || done; });
                if (done && tasks.empty())
                {
                    //cout << "thread terminate"<< endl;
                    break;
                }
                task = tasks.front();
                tasks.pop();
               // cout << "task value " << task << endl;
            }
          parfor1(task,task+chunk_size,1,f);
          
      }
  }

  template <typename TLS>
  void parfor(size_t beg, size_t end, size_t increment, size_t nthreads,
            size_t chunk_size, std::function<void(TLS &)> before,
              std::function<void(int, TLS &)> func,
              std::function<void(TLS &)> after)
  {
    int task_complete = 0;
    vector<thread> thread_stack;
    vector<TLS> threadContextStorage(nthreads);
    int counter = 0;
    for (int t  = 0;t< nthreads;t+=1){
      before(threadContextStorage[t]);
    }
    for (int i = beg; i < end; i +=  chunk_size)
    {
        this->push_items(i);
    }  
    for (int t = 0; t < nthreads; t += 1)
    {
      //before(threadContextStorage[t]);
      thread_stack.push_back(thread(&DynLoop::executor, this, chunk_size,[&, t](int j) -> void {
        func(j, threadContextStorage[t]);
      }));
    }
    this->all_done();
    
    for (auto &itr : thread_stack)
    {
      itr.join();
    }
    for (auto &itr : threadContextStorage)
    {
      after(itr);
    };
    
 }
};

#endif

