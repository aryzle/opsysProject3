#include <iostream>
#include <fstream>
#include <string>
#include "Memory.h"

int main (int argc, char* argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " input" << endl;
    return 1;
  }
  string line;
  deque<Memory> procs_cont;
  deque<Memory> procs_ncont;
  int num_procs = 0;
  ifstream iFile;
  iFile.open(argv[1]);
  if (iFile.is_open()) {
    getline(iFile, line);
    num_procs = stoi(line);     //get N ex: "20"
    while (getline(iFile, line)) {
      create_proc(line);
    }
    iFile.close();
  } else {
    cerr << "couldn't open file " << argv[1] << endl;
    return 1;
  }
  //TODO: code to run contiguous and then non-contiguous sims
  return 0;
}

void create_proc (string line) {
  string s;
  vector<int> tokens;
  char name = line[0];    //get name 'A'-'Z'
  line.erase(0,2);        //erase name and following space from line
  istringstream iss(line);
  while (getline(iss, s, ' '))
    tokens.push_back(atoi(s.c_str()));
  //TODO: code to create process and add to both deques
}
