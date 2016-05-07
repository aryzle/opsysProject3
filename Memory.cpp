#include <iostream>
#include <string.h>
#include <deque>
#include "Proc.h"
#include "Memory.h"
using namespace std;

// Memory default copnstructor
Memory::Memory () {
  size = 256;
  empty = 256;
  memset(frames, '.', sizeof(frames));
  partitions.push_back(pair<int, int> (0, size-1));
}

void Memory::print () const {
  cout << string(32, '=');
  for (int i=0; i<size; i++) {
    if (i%32 == 0) cout << endl;
    cout << frames[i];
  }
  cout << endl << string(32, '=') << endl;
}

void Memory::add (Proc& p, int& time) {
  int part_index;
  vector<Proc>::iterator itr;
  time = p.arrival_t;
  cout << "time " << time << "ms: Process " << p.name << " arrived (requires "
       << p.memory << " frames of physical memory)" << endl;
  part_index = check(p, time);
  if (part_index > -1) {
    p.mem_b = partitions[part_index].first;
    partitions[part_index].first = p.mem_b + p.memory;
    empty -= p.memory;
    if (partitions[part_index].first == partitions[part_index].second)
      partitions.erase(partitions.begin() + part_index);
    for (int j=p.mem_b; j<p.mem_b+p.memory; j++)
      frames[j] = p.name;
    cout << "time " << time << "ms: Placed process " << p.name << " in "
         << "memory:" << endl;
    for (itr = procs.begin(); itr!=procs.end(); itr++) {
      if (p.exit_t < itr->exit_t) {
        procs.insert(itr, p);
        return;
      }
    }
    procs.push_back(p);
  }
}

// return -1 if not enough memory, otherwise returns where it should begin
int Memory::check (Proc p, int time) {
  if (p.memory > empty) {
    cout << "time " << time << "ms: Cannot place process " << p.name << " -- "
         << "skipping process " << p.name << endl;
    return -1;
  }
  int i;
  for (i=0; i<partitions.size(); i++) {
    if (partitions[i].second - partitions[i].first < p.memory)
      continue;
    return i;
  }
  cout << "time " << time << "ms: Cannot place process " << p.name << " -- "
       << "starting defragmentation" << endl;
  defrag(time);
  return -1;
}

void Memory::defrag(int time) {
  int start_ind=0;
  int end_ind=0;
  int end_let=0;
  int ind = 32;
  int moved = 0;
  char temp[256];
  //TODO: Figure out why it is off by a few 
  
  //while(frames[ind] != '='){
    while (frames[ind] != '.'){       //go through mem array until '.'
      ind++; 
    }
    start_ind = ind;
    while (frames[ind] == '.'){     //goes until another proc or the end
      ind++;
    }
    end_ind = ind;
    while(frames[ind] != '.' && frames[ind] != '='){
      ind++;
    }
    end_let = ind;
    int temp_i = 0;
    int let_moved = end_let-end_ind;
    while(start_ind != end_let-(end_ind-start_ind)){
      temp[temp_i] = frames[end_ind];
      frames[start_ind] = frames[end_ind];
      start_ind++;
      end_ind++;
      temp_i++;
    }
    
  moved = end_ind - start_ind;
  memset(frames+(end_let-moved), '.', moved);
  //}
  char c;
  //TODO: adjust proc start and end places inside of frames
  for(int i = 0; i < size; ++i){
    c = temp[i];
    while(temp[i] == c){
      i++; 
    }
    
    
  }

  time += moved;
  cout << "time " << time << ": Defragmentation complete (moved " << let_moved << " frames " << endl;
  //print();
}

// check processes for completion
void Memory::complete (int& time, int arrival_t) {
  vector<Proc>::iterator itr, temp;
  for (itr = procs.begin(); itr != procs.end(); ++itr) {
    if (arrival_t > itr->exit_t) {
      cout << "time " << itr->exit_t << "ms: Process " << itr->name
           << " removed from physical memory" << endl;
      partitions.push_back(pair<int, int> (itr->mem_b, itr->mem_b+itr->memory));
      empty += itr->memory;
      for (int j=itr->mem_b; j<itr->mem_b+itr->memory; j++)
        frames[j] = '.';
      temp = itr;
      itr--;
      procs.erase(temp);
      print();
    }
  }
}
