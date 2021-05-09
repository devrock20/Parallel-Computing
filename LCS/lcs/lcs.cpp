#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include "cmath"
#include "par_loop.hpp"
using namespace std;
#ifdef __cplusplus
extern "C"
{
#endif

  void generateLCS(char *X, int m, char *Y, int n);
  void checkLCS(char *X, int m, char *Y, int n, int result);

#ifdef __cplusplus
}
#endif

int LCS(char *X, int m, char *Y, int n, int nthreads)
{

  int **C = new int *[m + 1];
  for (int i = 0; i <= m; ++i)
  {
    C[i] = new int[n + 1];
    C[i][0] = 0;
  }
  for (int j = 0; j <= n; ++j)
  {
    C[0][j] = 0;
  }

  auto func = [&](int i, int j) -> void {
    if (X[i] == Y[j])
    {
      C[i + 1][j + 1] = C[i][j] + 1;
    }
    else
    {
      C[i + 1][j + 1] = std::max(C[i][j + 1], C[i + 1][j]);
    }
  };
  OmpLoop obj;
  obj.setNbThread(nthreads);
  //top to disgonal
  for (int k = 1; k < m; k++)
  {
    obj.parfor(
        k, 1,
        [&](int i, int j) -> bool { return i > 0 && j < n; },
        [&](int i, int j) -> void {
          func(i, j);
        });
  }
  //diagonal to bottom
  for (int c = 1; c < n; c++)
  {
    obj.parfor(
        m - 1, c,
        [&](int i, int j) -> bool { return i > 0 && j < n; },
        [&](int i, int j) -> void {
          func(i, j);
        });
  }

  int result = C[m][n];
  for (int i = 0; i <= m; ++i)
  {
    delete[] C[i];
  }
  delete[] C;
  return result;
}

int main(int argc, char *argv[])
{

  if (argc < 4)
  {
    std::cerr << "usage: " << argv[0] << " <m> <n> <nbthreads>" << std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nthreads = atoi(argv[3]);

  // get string data
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  int lcs = LCS(X, m, Y, n, nthreads);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end - start;

  std::cerr << elpased_seconds.count() << std::endl;

  delete[] X;
  delete[] Y;

  checkLCS(X, m, Y, n, lcs);

  return 0;
}
