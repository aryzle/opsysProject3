#include <iostream>
#include <string.h>
#include "Proc.h"
#include "Memory.h"
using namespace std;

// Memory default copnstructor
Memory::Memory() {
  size = 256;
  empty = 256;
  memset(frames, '.', sizeof(frames));
  partitions.push_back(pair<int, int> (0, size-1));
}

void Memory::print() const {
  cout << string(32, '=');
  for (int i=0; i<size; i++) {
    if (i%32 == 0) cout << endl;
    cout << frames[i];
  }
  cout << endl << string(32, '=') << endl;
}

void Memory::add(Proc p) {
  if (check(p.memory)) {
    for (int i=0; i<partitions.size(); i++) {
      if (partitions[i].second - partitions[i].first < p.memory)
        continue;
      p.mem_b = partitions[i].first;
      partitions[i].first = p.mem_b + p.memory;
      if (partitions[i].first == partitions[i].second) // partition empty
        partitions.erase(partitions.begin() + i);
      for (int j=p.mem_b; j<p.memory; j++) {
        frames[j] = p.name;
      }
    }
    procs.push_back(p);
  }
}

bool Memory::check(int mem) {
  cout << mem << endl;
  return true;
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
