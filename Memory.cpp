#include <iostream>
#include <string.h>
#include "Memory.h"
using namespace std;

// Memory default copnstructor
Memory::Memory() {
  size = 256;
  empty = 256;
  memset(frames, '.', sizeof(frames));
}

void Memory::print() const {
  cout << string(32, '=');
  for (int i=0; i<size; i++) {
    if (i%32 == 0) cout << endl;
    cout << frames[i];
  }
  cout << endl << string(32, '=') << endl;
}

void Memory::defrag() {
  int ind=0;
  int ind2=0;
  //TODO: Figure out how to do defrag
  while (frames[ind] != '.'){       //go through mem array until '.'
    ind++;           
    while (frames[ind] == '.'){     //goes until another proc or the end
      ind2++;
    }
    //--have to delete the periods in middle of procs in mem and move
    //                the procs after the '.'s to take their place
    //--have to keep track of which processes are there? or.....?
    //--have to know how many frames were moved and increment the timer accordingly
    
  }
  
}