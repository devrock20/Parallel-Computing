#include <iostream>
#include  <thread>
#include <string>
#include <vector>

using namespace std;

void minion(int m){
  cout << "Hello! I am minion "<< m << "\n";
}

void  overlord_thread(){
  cout << "hello minions! I am the Overlord!";
}


int main (int argc, char** argv) {
  if (argc < 2) {
    cerr<<"usage: "<<argv[0]<<" <nbminions>\n";
    return -1;
  }
  float m = stoi(argv[1]);
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
  thread overlord_thread (overlord_thread)
  thread.join()

  
  return 0;
}
