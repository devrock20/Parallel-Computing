#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <string>
using namespace std;
using chrono::system_clock;


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

/**
 * @arguments: 
 * functionid, 
 * lower bound,
 * upper bound,
 * points for approximation,
 * intensity of computation
*/
int main (int argc, char* argv[]) {
  int functionid;
  float a,b,n,intensity;

  if (argc < 6) 
  {
    cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
  else if (argc == 6)
  {
   	functionid = std::stoi(argv[1]);
   	a  = std::stoi(argv[2]);
   	b = std::stoi(argv[3]);
   	n = std::stoi(argv[4]);
   	intensity = std::stoi(argv[5]);
   	float  sum =0.000;
   	float answer = 0.000;
   	chrono::seconds sec(1);
   	
    switch (functionid)
   	{
   		case 1:
   		for (int i=0;i<=n-1;i++){
   			float function = (a+(i+0.5)*((b - a)/n));
   			system_clock::time_point before_integration = system_clock::now();
   			float integral = f1(function,intensity);
   			system_clock::time_point after_integration =  system_clock::now();
   			sum = sum + integral;
   		    chrono::system_clock::duration time_taken = after_integration - before_integration;
   		    cerr << time_taken.count();
   	         
   		}
   		answer  = ((b - a)/n) * sum;
   		cout << answer;
   		return 0;
   		case 2:
   		for (int i=0;i<=n-1;i++){
   		        float function = (a+i+0.5)*((b - a)/n);
   			system_clock::time_point before_integration = system_clock::now();
   			float integral = f2(function,intensity);
   			system_clock::time_point after_integration =  system_clock::now();
   			sum = sum + integral;
   		        chrono::system_clock::duration time_taken = after_integration - before_integration;
   		        cerr << time_taken.count();
   		}
   		answer  = ((b - a)/n) * sum;
   		cout << answer;
   		return 0;
   		case 3:
   		for (int i=0;i<=n-1;i++){
   		        float function = (a+i+0.5)*((b - a)/n);
   			system_clock::time_point before_integration = system_clock::now();
   			float integral = f3(function,intensity);
   			system_clock::time_point after_integration =  system_clock::now();
   			sum = sum + integral;
   		        chrono::system_clock::duration time_taken = after_integration - before_integration;
   		        cerr << time_taken.count();
   		}
   		answer  = ((b - a)/n) * sum;
   		cout << answer;
   		return 0;
   		case 4:
   		for (int i=0;i<=n-1;i++){
   			float function = (a+i+0.5)*((b - a)/n);
   			system_clock::time_point before_integration = system_clock::now();
   			float integral = f4(function,intensity);
   			system_clock::time_point after_integration =  system_clock::now();
   			sum = sum + integral;
   		        chrono::system_clock::duration time_taken = after_integration - before_integration;
   		        cerr << time_taken.count();
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
