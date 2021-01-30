#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <string>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

  
int main (int argc, char* argv[]) {
  int functionid,a,b,n,intensity;

  if (argc < 6) 
  {
    cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
  else if (argc == 6)
  {
   	functionid = atoi(argv[1]);
   	a  = atoi(argv[2]);
   	b = atoi(argv[3]);
   	n = atoi(argv[4]);
   	intensity = atoi(argv[5]);
   	float  sum =0.0;
   	float answer = 0.0;
    switch (functionid)
   	{
   		case 1:
   		for (int i=0;i<n-1;i++){
   			float f = a+(i*0.5)*((b - a)/n);
   			float function = f1(f,intensity);
   			sum = sum + function;
   		}
   		answer  = ((b - a)/n) * sum;
   		cout << answer;
   		return 0;
   		case 2:
   		for (int i=0;i<n-1;i++){
   			float f = a+(i*0.5)*((b - a)/n);
   			float function = f2(f,intensity);
   			sum = sum + function;
   		}
   		answer  = ((b - a)/n) * sum;
   		cout << answer;
   		return 0;
   		case 3:
   		for (int i=0;i<n-1;i++){
   			float f = a+(i*0.5)*((b - a)/n);
   			float function = f3(f,intensity);
   			sum = sum + function;
   		}
   		answer  = ((b - a)/n) * sum;
   		cout << answer;
   		return 0;
   		case 4:
   		for (int i=0;i<n-1;i++){
   			float f = a+(i*0.5)*((b - a)/n);
   			float function = f4(f,intensity);
   			sum = sum + function;
   		}
   		answer  = ((b - a)/n) * sum;
   		cout << answer;
   		return 0;
   	} 
  }
  else {
  	cerr<<"Incorrect number of arguments passed usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
  return 0;
}
