#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include "seq_loop.hpp"
#include "thread"
#include <vector>
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

//abstract funtion
typedef float (*absFun)(float x, int intensity);
SeqLoop sl;

void single_thread_action(int i, float n, int nbthreads, float a, float b, absFun function, float intensity, float &sum)
{

  sl.parfor<float>(
      i, n, nbthreads,
      [&](float &localSum) -> void {
        localSum = 0.00;
      },
      [&](int count, float &localSum) -> void {
        float expression = (a + (count + 0.5) * ((b - a) / n));
        float integral = function(expression, intensity);
        localSum += integral;
      },
      [&](float &localSum) -> void {
        sum += localSum;
      });
}

float getSummation(absFun function, float a, float b, float n, float intensity, int nbthreads)
{
  float sum = 0.000;
  vector<thread> filethreads;
  sl.parfor(
      0, nbthreads, 1,
      [&](int i) -> void {
        filethreads.push_back(thread(single_thread_action, i, n, nbthreads, a, b, function, intensity, ref(sum)));
        filethreads.at(i).join();
      });
  return sum;
}

// calculates and displays the integral
void calcAndDisplayIntegral(float sum, float a, float b, float n)
{
  cout << ((b - a) / n) * sum << endl;
}

// calculates and displays time difference
void calcAndDisplayTime(chrono::time_point<chrono::system_clock> before_integration)
{
  auto after_integration = chrono::system_clock::now();
  float time_duration = chrono::duration_cast<chrono::milliseconds>(after_integration - before_integration).count();
  float time_diff_sec = time_duration / 60;
  cerr << time_diff_sec;
}

int main(int argc, char *argv[])
{

  if (argc < 7)
  {
    std::cerr << "usage: " << argv[0] << " <functionid> <a> <b> <n> <intensity> <nbthreads>" << std::endl;
    return -1;
  }
  int functionid = stoi(argv[1]);
  float a = stoi(argv[2]);
  float b = stoi(argv[3]);
  float n = stoi(argv[4]);
  float intensity = stoi(argv[5]);
  int nbthreads = stoi(argv[6]);
  // int functionid = 1;
  // float a = 0;
  // float b = 10;
  // float n = 1000;
  // float intensity = 1;
  // int nbthreads = 2;
  float summation;
  auto before_integration = chrono::system_clock::now();
  switch (functionid)
  {
  case 1:
    summation = getSummation((absFun)f1, a, b, n, intensity, nbthreads);
    break;
  case 2:
    summation = getSummation((absFun)f2, a, b, n, intensity, nbthreads);
    break;
  case 3:
    summation = getSummation((absFun)f3, a, b, n, intensity, nbthreads);
    break;
  case 4:
    summation = getSummation((absFun)f4, a, b, n, intensity, nbthreads);
    break;
  default:
    cerr << "Unknown function id passed: " << functionid << endl;
  }
  calcAndDisplayIntegral(summation, a, b, n);
  calcAndDisplayTime(before_integration);
  return 0;
}
