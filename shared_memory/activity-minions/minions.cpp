#include <iostream>
#include  <thread>

void minion(int m){
	std::cout << "Hello! I am minion "<< m
}


int main (int argc, char** argv) {
  if (argc < 2) {
    std::cerr<<"usage: "<<argv[0]<<" <nbminions>\n";
    return -1;
  }
  float m = stoi(argv[0]);
  std::vector <std::thread > minion_threads;
  for (int i=0; i<m; ++i) 
  	{ 
  		std:: thread  minion_thread (minion,i);
  		minion_threads.push_back(std::move(minion_thread ));
  	}
  for (auto & t : minion_threads)
   {
   	t.join ();
  }
  
  
  return 0;
}
