#include "Memory.h"
using namespace std;

// Memory default copnstructor
Memory::Memory() {
  size = 256;
  empty = 256;
  memset(frames, '.', sizeof(frames));
}

void Memory::print() const {
  cout << string(32, '=') << endl;
  for (int i=0; i<size; i++) {
    if (i%32 == 0) cout << endl;
    cout << frames[i];
  }
  cout << endl << string(32, '=') << endl;
}
