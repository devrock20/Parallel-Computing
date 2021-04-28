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

void printArr(int **arr, int m, int n)
{
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < n; j++)
    {
      cout << arr[i][j] << "\t";
    }
    cout << endl;
  }
}

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
  chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
  int *pr = new int[n + 1];

  int levels = log2(n) + 1;

  int **multilevelArr = new int *[levels + 2];
  for (int i = 0; i < levels + 2; ++i)
    multilevelArr[i] = new int[n];

  for (int i = 0; i < n; i++)
  {
    multilevelArr[0][i] = arr[i];
  }

  staticFor(1, levels, 1, nthreads, [&](int level) -> void {
    for (int i = 0; i < n; i++)
    {
      multilevelArr[level][i] = multilevelArr[level - 1][i];
    }

    for (int k = pow(2, level - 1); k < n; k++)
    {
      if (k >= pow(2, level - 1))
      {
        int index = k - pow(2, level - 1);
        multilevelArr[level][k] = multilevelArr[level - 1][index] + multilevelArr[level - 1][k];
      }
    }
    // if I print something in this lambda function then the last itertaion is stored inside the array
    // cout<< "Level" << level <<endl;
  });
  
  printArr(multilevelArr, levels, n);

  /**
   * please ignore below
  */
  // for (int level = 1; level <= levels; level++)
  // {

  //   for (int i = 0; i < n; i++)
  //   {
  //     multilevelArr[level][i] = multilevelArr[level - 1][i];
  //   }

  //   for (int k = pow(2, level - 1); k < n; k++)
  //   {
  //     if (k >= pow(2, level - 1))
  //     {
  //       int index = k - pow(2, level - 1);
  //       multilevelArr[level][k] = multilevelArr[level - 1][index] + multilevelArr[level - 1][k];
  //     }
  //   }
  // }
  pr[0] = 0;
  for (int i = 0; i < n; i++)
  {
    pr[i + 1] = multilevelArr[levels - 1][i];
  }
  chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();

  chrono::duration<double> elapsed_seconds = end - start;

  cerr << elapsed_seconds.count() << endl;

  checkPrefixSumResult(pr, n);

  return 0;
}
