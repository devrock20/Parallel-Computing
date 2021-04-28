#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>


#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif



int main (int argc, char* argv[]) {

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);

  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);
  int** C = new int*[m+1];
  for (int i=0; i<=m; ++i) {
    C[i] = new int[n+1];
    C[i][0] = 0;
  }
  for (int j=0; j<=n; ++j) {
    C[0][j] = 0;
  }
  
int Lmax=0;
int count =0;
for(int i;i<=m;i++) {
	for(int j=1;j<=i;j++) {
		int k=i-(j-1);
		if(Y[k-1]==X[j-1]) {
			C[k][j]=C[k-1][j-1]+1;
			if(C[k][j]>Lmax) {
				Lmax=C[k][j];
				// parent[s]=k-1;
				// s++;
				count++;
			}
		} else if(C[k-1][j]>=C[k][j-1]) {
			C[k][j]=C[k-1][j];
		} else {
			C[k][j]=C[k][j-1];
		}
	}
}
  int result = -1; // length of common subsequence

result = Lmax;
  checkLCS(X, m, Y, n, result);


  return 0;
}
