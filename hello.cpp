#include <iostream>
#include <unistd.h>
using namespace std;
int main () {
  char name[256];
	int hostname;
  hostname=gethostname(name, sizeof(name));
  cout >> hostname;
  return 0;
}
