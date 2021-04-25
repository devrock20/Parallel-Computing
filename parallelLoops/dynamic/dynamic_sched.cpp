#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dyn_loop.hpp"
using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif

  float f1(float x, int intensity);
  float f2(float x, int intensity);
  float f3(float x, int intensity);
  float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

void numerical_function(int functionid,float a, float b,float n,float intensity){
     int  i = 0;
     float tls = 0.0;
     float sum = 0;                  
     float x = (a + (i + 0.5) * ((b - a) / n));
        switch (functionid)
        {
        case 1:
          tls += f1(x, intensity);
          break;
        case 2:
          tls += f2(x, intensity);
          break;
        case 3:
          tls += f3(x, intensity);
          break;
        case 4:
          tls += f4(x, intensity);
          break;
        }
        sum += tls;
    return sum;
}

int main (int argc, char* argv[]) {

  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <granularity>"<<std::endl;
    return -1;
  }

  int functionid = stoi(argv[1]);
  float a = stoi(argv[2]);
  float b = stoi(argv[3]);
  float n = stoi(argv[4]);
  float intensity = stoi(argv[5]);
  int nbthreads = stoi(argv[6]);
  int granularity = stoi(argv[7]);
  int no_of_iterations = n/granularity;
  int common_expression = ((b - a) / n);
  float sum = 0.0;
  std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

  DynLoop d;
  vector<thread> thread_pool;
  for (int i =0;i<nbthreads;i++){
    thread_pool.push_back(thread(&DynLoop::initial_run, &d));
  }

  for (int s = 0;s<n;s += no_of_iterations){
    d.push(
      numerical_function(functionid,a,b,s,intensity));
  }
  d.is_done();
  for (auto &itr : thread_pool)
    {
      itr.join();
    }

  std::cout << ((b - a) / n) * sum << endl;

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - begin;

  std::cerr << elapsed_seconds.count() << std::endl;

  return 0;
}
