#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Memory.h"
#include "Proc.h"
#include "deque.h"

void run (deque<Proc> &procs, int n, string scheme);
void create_proc (string line, deque<Proc> &p);

int main (int argc, char* argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " input" << endl;
    return 1;
  }
  string line;
  deque<Proc> procs_cont;
  deque<Proc> procs_ncont;
  int num_procs = 0;
  ifstream iFile;
  iFile.open(argv[1]);
  if (iFile.is_open()) {
    getline(iFile, line);
    num_procs = stoi(line);     //get N ex: "20"
    while (getline(iFile, line)) {
      create_proc(line, procs_cont);
      create_proc(line, procs_ncont);
    }
    iFile.close();
  } else {
    cerr << "couldn't open file " << argv[1] << endl;
    return 1;
  }
  /*for (int i=0; i<procs_cont.size(); i++) {
    procs_cont[i].print();
    procs_ncont[i].print();
  }*/
  
  run(procs_cont, num_procs, "Contiguous");
  run(procs_ncont, num_procs, "Non-contiguous");
  return 0;
}

//
//TODO: code to run contiguous and non-contiguous sims
//
void run (deque<Proc> &procs, int n, string scheme) {
  Memory m = Memory(); 
  Proc p;
  int time = 0;
  const int t_memmove = 1;
  //m.print();
  
  if (scheme == "Contiguous"){
    cout << "time " << time << "ms: Simulator started (" << scheme << " -- "
         << "First-Fit)" << endl;
    while (!procs.empty()) {
      p = procs.front();
      procs.pop_front();
      m.add(p);
      m.print();
    }
  }
  else if(scheme ==  "Non-contiguous"){
    //do non contiguous 
  }
  return;
}

// takes a string "A 45 0-350 400-450 .." and a deque
//
void create_proc (string line, deque<Proc> &p) {
  string s, s2;
  vector<int> tokens, times(3, -1);
  char name = line[0];    //get name 'A'-'Z'
  line.erase(0,2);        //erase name and following space from line
  istringstream iss(line);
  while (getline(iss, s, ' ')) {          //this is to break down diff times
    istringstream iss2(s);
    while (getline(iss2, s2, '-'))        //this is to break up 0-350
      tokens.push_back(atoi(s2.c_str()));
  }
  times[0] = tokens[0];
  for (int i=1; i<tokens.size(); i+=2) {
    times[1] = tokens[i];
    times[2] = tokens[i+1];
    push_deque(p, Proc(name, times));
  }
}
