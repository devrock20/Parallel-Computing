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

//abstract funtion
typedef float (* absFun)(float x, int intensity);

// returns the sum of the expression while calling the appropriate Integral function
float getSummation (absFun function, float a, float b, float n, float intensity) {
  float sum =0.000;
  float answer = 0.000;
  for (int i=0; i<=n-1; i++){
  	float expression = (a + (i + 0.5)*((b - a)/n));
  	float integral = function(expression,intensity);
  	sum = sum + integral;   
  }
  return sum;
}

// calculates and displays the integral
void calcAndDisplayIntegral(float sum, float a, float b, float n) {
  cout << ((b - a)/n) * sum;
}

// calculates and displays time difference
void calcAndDisplayTime(chrono::time_point<chrono::system_clock> before_integration){
   auto after_integration = chrono::system_clock::now();
   cerr << chrono::duration_cast < chrono::seconds> (after_integration  - before_integration).count();

}


int main (int argc, char* argv[]) {
  //safety checks
  if (argc < 6) {
    cerr <<"usage: "<< argv[0] <<" <functionid> <a> <b> <n> <intensity>"<< endl;
    return -1;
  }
  if(argc > 6) {
	cerr <<"Incorrect number of arguments passed usage: "<< argv[0] <<" <functionid> <a> <b> <n> <intensity>"<< endl;
    return -1;
  }
  int functionid = stoi(argv[1]);
  float a = stoi(argv[2]);
  float b = stoi(argv[3]);
  float n = stoi(argv[4]);
  float intensity = stoi(argv[5]);
  
  float summation;
  auto before_integration = chrono::system_clock::now();
  
  
  switch (functionid) {
  	case 1:
  	  summation = getSummation((absFun)f1, a, b, n, intensity);
      calcAndDisplayIntegral(summation, a, b, n);
       calcAndDisplayTime(before_integration);
  	  break;
  	case 2:
  	  summation = getSummation((absFun)f2, a, b, n, intensity);
      calcAndDisplayIntegral(summation, a, b, n);
      calcAndDisplayTime(before_integration);
  	  break;
  	case 3:
  	  summation = getSummation((absFun)f3, a, b, n, intensity);
      calcAndDisplayIntegral(summation, a, b, n);
      calcAndDisplayTime(before_integration);
  	  break;
  	case 4:
  	  summation = getSummation((absFun)f4, a, b, n, intensity);
      calcAndDisplayIntegral(summation, a, b, n);
      calcAndDisplayTime(before_integration);
  	  break;
    default:
      cerr<<"Unknown function id passed: " << functionid << endl;
  } 
  return 0;
}
