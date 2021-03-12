#include <iostream>
#include  <thread>
#include <string>
#include <vector>

using namespace std;

void minion(int m){
  cout << "Hello! I am minion "<< m << "\n";
}


int main (int argc, char** argv) {
  if (argc < 2) {
    cerr<<"usage: "<<argv[0]<<" <nbminions>\n";
    return -1;
  }
  float m = stoi(argv[0]);
  vector <std::thread > minion_threads;
  for (int i=0; i<m; ++i) 
  	{ 
  		thread  minion_thread (minion,i);
  		minion_threads.push_back(std::move(minion_thread ));
  	}
  for (auto & t : minion_threads)
   {
   	t.join ();
  }
  thread overlord_thread ("hello minions! I am the Overlord!")
  thread.join()

  
  return 0;
}
