#include <iostream>
#include <unistd.h>
using namespace std;
int main () {
<<<<<<< HEAD
  char name[256];
	int hostname;
  hostname=gethostname(name, sizeof(name));
  cout >> hostname;
=======
  char machine_name[256];
  gethostname(machine_name,sizeof(machine_name));
  cout << machine_name << endl;
>>>>>>> 062cb1f50d172fe0ec141cfeacab2759fdc30c1b
  return 0;
}
