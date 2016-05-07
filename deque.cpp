#include <iostream>
#include <string>
#include <iterator>
#include "Proc.h"
#include "deque.h"
using namespace std;

typedef deque<Proc>::iterator p_d_itr;

void push_deque (deque<Proc>& procs, Proc p) {
  if (procs.empty()) {
    procs.push_back(p);
    return;
  }
  for (p_d_itr itr = procs.begin(); itr != procs.end(); ++itr) {
    if (p.arrival_t < itr->arrival_t) {
      procs.insert(itr, p);
      return;
    }
    if (p.arrival_t == itr->arrival_t && p.name < itr->name) {
      procs.insert(itr, p);
      return;
    }
  }
  procs.push_back(p);
}

/*void push_sjf_deque (deque<Proc>& procs, Proc p) {
  if (procs.empty()) {
    procs.push_back(p);
    return;
  }
  for (p_d_itr itr = procs.begin(); itr != procs.end(); ++itr) {
    if (p.burst_t < itr->burst_t) {
      procs.insert(itr, p);
      return;
    }
  }
  procs.push_back(p);
}*/

void print_deque (deque<Proc> procs) {
  for (p_d_itr itr = procs.begin(); itr != procs.end(); ++itr)
    cout << " " << itr->name;
  cout << "]" << endl;
}


void change_times(deque<Proc>& procs, int& offset){
  for (p_d_itr itr = procs.begin(); itr != procs.end(); ++itr) {
    itr->arrival_t += offset;
    itr->exit_t += offset;
  }
  
}
