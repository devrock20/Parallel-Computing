#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <chrono>
#include "par_loop.hpp"

using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif
  void generatePrefixSumData(int *arr, size_t n);
  void checkPrefixSumResult(int *arr, size_t n);
#ifdef __cplusplus
}
#endif

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    cerr << "Usage: " << argv[0] << " <n> <nbthreads>" << endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nthreads = atoi(argv[2]);
  int *arr = new int[n];
  generatePrefixSumData(arr, n);
  int *pr = new int[n + 1];
  OmpLoop obj;
  obj.setNbThread(nthreads);
  int levels = log2(n) + 1;
  //initalizing table for a dynamic approach
  int **multilevelArr = new int *[levels + 2];
  for (int i = 0; i < levels + 2; ++i)
    multilevelArr[i] = new int[n];
  chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
  //copying elements from arr to first row
  for (int i = 0; i < n; i++) {
    multilevelArr[0][i] = arr[i];
  }
  
  for (int level = 1; level <= levels; level++)
  {
    //copying values from before row  
    obj.parfor(0,n,1,[&](int i)->void{
      multilevelArr[level][i] = multilevelArr[level - 1][i];
    });
    //prefixing sum
    obj.parfor(pow(2, level - 1), n, 1,[&](int k) -> void {
      if (k >= pow(2, level - 1))
      {
        int index = k - pow(2, level - 1);
        multilevelArr[level][k] = multilevelArr[level - 1][index] + multilevelArr[level - 1][k];
      }
    });
  }

  chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  pr[0] = 0;
  //constructing the original array
  for (int i = 0; i < n; i++)
  {
    pr[i + 1] = multilevelArr[levels][i];
  }
  delete[][] multilevelArr;
  chrono::duration<double> elapsed_seconds = end - start;
  cerr << elapsed_seconds.count() << endl;
  checkPrefixSumResult(pr, n);
  return 0;
}
