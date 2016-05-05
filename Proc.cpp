#include <iostream>
#include <vector>
#include "Proc.h"
using namespace std;

//Proc default constructor
Proc::Proc() {
  name = 0;
  memory = -1;
  arrival_t = -1;
  exit_t = -1;
}

//Proc constructor: takes a char for name then a vector with 3 ints
//                  for it's member variables
Proc::Proc (char c, vector<int> values) {
  name = c;
  memory = values[0];
  arrival_t = values[1];
  exit_t = values[2];
}

void Proc::print() const {
  cout << name << " " << memory << " " << arrival_t << " " << exit_t << endl;
}
