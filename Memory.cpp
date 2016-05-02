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
  // need to print array still
  cout << string(32, '=') << endl;
}
