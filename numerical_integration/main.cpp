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
   	float  sum = 0;
   	functionid = atoi(argv[1]);
   	a  = atoi(argv[2]);
   	b = atoi(argv[3]);
   	n = atoi(argv[4]);
   	intensity = atoi(argv[5]);

   for (int i=0;i<=n-1;i++)
   		{
   			float function = a+(i*0.5)*((b - a)/n);
   			sum = sum + function;
   		}

   float x = ((b - a)/n) * sum;
   switch (functionid)
   	{
   		case 1:
   		f1(x,intensity);
   		case 2:
   		f2(x,intensity);
   		case 3:
   		f3(x,intensity);
   		case 4:
   		f4(x,intensity);
   	} 
  }
  return 0;
}
