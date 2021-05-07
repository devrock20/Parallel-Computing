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

  // for (int i = 0; i < m; i++)
  // {
  //   cout << X[i] << "\t";
  // }
  // cout << endl;
  // for (int j = 0; j < m; j++)
  // {
  //   cout << Y[j] << "\t";
  // }

  // cout << endl;
  auto func = [&](int i, int j) -> void {
    if (X[i] == Y[j])
    {
      // cout << i << "|" << j << " +1" << endl;

      C[i + 1][j + 1] = C[i][j] + 1;
    }
    else
    {
      // cout << i << "|" << j << " max";
      C[i + 1][j + 1] = std::max(C[i][j + 1], C[i + 1][j]);
    }
  };
  OmpLoop obj;
  obj.setNbThread(nthreads);
  for (int k = 1; k < m; k++)
  {
    // int i = k, j = 1;
    // staticFor(
    //     0, 0, [&]() -> bool { return i > 0 && j < n; }, 0, nthreads, [&]() -> void {
    //   func(i, j);
    //   i--; });
    // int i = k, j = 1;
    obj.parfor(
        k, 1,
        [&](int i, int j) -> bool { return i > 0 && j < n; },
        [&](int i, int j) -> void {
          func(i, j);
        });
    // parfor(
    //     k, 1,
    //     [&](int i, int j) -> bool { return i > 0 && j < n; },
    //     // [](int i, int j) -> void { i--, j++; },
    //     [&](int i, int j) -> void {
    //       func(i, j);
    //       //             for (int i = 0; i <= m; i++)
    //       // {
    //       //   for (int j = 0; j <= n; j++)
    //       //   {
    //       //     cout << C[i][j] << "\t";
    //       //   }
    //       //   cout << endl;
    //       // }
    //     },
    //     nthreads);
    // for (int i = k, j = 1;i > 0 && j < n; ; i--, j++)
    // {
    //   func(i, j);
    // }
    // cout << endl;
    // cout << "row number" << k << endl;
  }
  // cout << endl;

  // cout << "phase 2" << endl;
  for (int c = 1; c < n; c++)
  {
    obj.parfor(
        m - 1, c,
        [&](int i, int j) -> bool { return i > 0 && j < n; },
        [&](int i, int j) -> void {
          func(i, j);
        });
    // parfor(
    //     m - 1, c,
    //     [&](int i, int j) -> bool { return i > 0 && j < n; },
    //     [](int i, int j) -> void { i--, j++; },
    //     [&](int i, int j) -> void {
    //       func(i, j);
    //     },
    //     nthreads);
    // for (int j = c, i = m - 1; j < n && i > 0; j++, i--)
    // {
    // func(i,j);

    // if (X[i] == Y[j])
    // {
    //   cout << i << "|" << j << " +1" << endl;

    //   C[i + 1][j + 1] = C[i][j] + 1;
    // }
    // else
    // {
    //   // cout << i << "|" << j << " max";
    //   C[i + 1][j + 1] = std::max(C[i][j + 1], C[i + 1][j]);
    // }
    // }
    // cout << endl;
    // cout << "column number" << c << endl;
  }

  int result = C[m][n];
  // cout << "Finish" << result << endl;
  // for (int i = 0; i <= m; i++)
  // {
  //   for (int j = 0; j <= n; j++)
  //   {
  //     cout << C[i][j] << "\t";
  //   }
  //   cout << endl;
  // }

  // cout << "Rsult" << m-1 << n-1 << "\t" << C[m-1][n-1] << endl;

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

  // cout << endl;
  //insert LCS code here.
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  int lcs = LCS(X, m, Y, n, nthreads);
  // cout << lcs << "Resultasdasd" << endl;
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end - start;

  // checkLCS(X, m, Y, n, lcs);
  std::cerr << elpased_seconds.count() << std::endl;

  delete[] X;
  delete[] Y;

  // int result = -1; // length of common subsequence
  checkLCS(X, m, Y, n, lcs);

  return 0;
}