#include <iostream>
#include <unistd.h>
using namespace std;
int main () {
  char machine_name[256];
  gethostname(machine_name,sizeof(machine_name));
  cout << machine_name << endl;
  return 0;
}
