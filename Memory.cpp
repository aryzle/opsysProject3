#include <iostream>
#include <string.h>
#include <deque>
#include <algorithm>    
#include "Proc.h"
#include "Memory.h"
using namespace std;

// Memory default copnstructor
Memory::Memory () {
  size = 256;
  empty = 256;
  end_last_frame = 0;
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

int Memory::add (Proc& p, int& time, string& algo) {
  int offset;
  int part_index;
  vector<Proc>::iterator itr;
  time = p.arrival_t;
  cout << "time " << time << "ms: Process " << p.name << " arrived (requires "
       << p.memory << " frames of physical memory)" << endl;
  part_index = check(p, time, algo);
  if (part_index > -1) {
    p.mem_b = partitions[part_index].first;
    partitions[part_index].first = p.mem_b + p.memory;
    empty -= p.memory;
    if (partitions[part_index].first == partitions[part_index].second)
      partitions.erase(partitions.begin() + part_index);
    for (int j=p.mem_b; j<p.mem_b+p.memory; j++)
      frames[j] = p.name;
    end_last_frame = p.mem_b + p.memory;
    cout << "time " << time << "ms: Placed process " << p.name << " in "
         << "memory:" << endl;
    for (itr = procs.begin(); itr!=procs.end(); itr++) {
      if (p.exit_t < itr->exit_t) {
        procs.insert(itr, p);
        return 0;
      }
    }
    procs.push_back(p);
    return 0;
  }
  if(part_index == -2){         //if defrag needs to happen
    offset = defrag(time);   //defrag happens and returns the offset
    change_times_mem(offset);
    p.arrival_t += offset;
    p.exit_t += offset;
    time = p.arrival_t;
    int new_part_index = check(p, time, algo);    //runs the check function
    p.mem_b = partitions[new_part_index].first;
    partitions[new_part_index].first = p.mem_b + p.memory;
    empty -= p.memory;
    if (partitions[new_part_index].first == partitions[new_part_index].second)
      partitions.erase(partitions.begin() + new_part_index);
    for (int j=p.mem_b; j<p.mem_b+p.memory; j++)
      frames[j] = p.name;
    
    cout << "time " <<time<< "ms: Placed process " << p.name << " in " 
      << "memory:" << endl;
      
    for (itr = procs.begin(); itr!=procs.end(); itr++) {
      if (p.exit_t < itr->exit_t) {
        procs.insert(itr, p);
        return offset;
      }
    }
    procs.push_back(p);
    return offset;
  }
  return 0;
}

// return -1 if not enough memory, return -2 if defrag,
// otherwise returns where it should begin
int Memory::check (Proc p, int time, string& algo) {
  if (p.memory > empty) {
    cout << "time " << time << "ms: Cannot place process " << p.name << " -- "
         << "skipping process " << p.name << endl;
    return -1;
  }
  int i;
  //combines adjacent partitions
  sort(partitions.begin(), partitions.end());
  if (algo == "First") {
    for (i=0;i<(signed)partitions.size(); i++) {
      if (partitions[i].second == partitions[i+1].first) {
        partitions[i].second = partitions[i+1].second;
        partitions.erase(partitions.begin()+i+1);
      }
    }
    for (i=0; i<partitions.size(); i++) {
      if (partitions[i].second - partitions[i].first < p.memory)
        continue;
      return i;
    }
  }
  else if (algo == "Next") {    //TODO: fix this
    int begin_i = 0;
    for (i=0; i<partitions.size(); i++) {
      if (end_last_frame < partitions[i].first)
        begin_i = i;
    }
    for (i=begin_i; i<partitions.size(); ++i) {
      if (partitions[i].second - partitions[i].first < p.memory)
        continue;
      return i;
    }
    for (i=0; i<begin_i; i++) {
      if (partitions[i].second - partitions[i].first < p.memory)
        continue;
      return i;
    }
  }
  else if (algo == "Best") {      //looks for the smallest partition big enough
    for (i=0;i<(signed)partitions.size(); i++) {
      if (partitions[i].second == partitions[i+1].first) {
        partitions[i].second = partitions[i+1].second;
        partitions.erase(partitions.begin()+i+1);
      }
    }
    int best = 256;
    int flag = 0;
    int best_i = 0;
    for (i=0; i<partitions.size(); i++) {
      if (partitions[i].second - partitions[i].first >= p.memory){
        if(partitions[i].second - partitions[i].first < best) {
          best = partitions[i].second - partitions[i].first;
          best_i = i;
        }
      }
      flag = 1;
    }
    if(flag == 1){
      return best_i;
    }
  }
  for (i=0;i<(signed)partitions.size(); i++) {
    if (partitions[i].second == partitions[i+1].first) {
      partitions[i].second = partitions[i+1].second;
      partitions.erase(partitions.begin()+i+1);
    }
  }
  cout << "time " << time << "ms: Cannot place process " << p.name << " -- "
       << "starting defragmentation" << endl;
  return -2;
}

// compacts memory used and creates one partition of free space, returns
// spaces it moved
int Memory::defrag(int time) {
  int start_ind=0, end_ind=0, end_let=0, ind=0, moved=0, let_moved;
  while (frames[ind] != '.')       //go through mem array until '.'
    ind++; 
  start_ind = ind;
  while (frames[ind] == '.')     //goes until another proc or the end
    ind++;
  end_ind = ind;
  while (frames[ind] != '.') //goes to end of letters
    ind++;
  end_let = ind;
  let_moved = end_let-end_ind;
  int temp_start = start_ind;
  moved = end_ind - start_ind;
  while (start_ind != end_let-(end_ind-start_ind)) {
    frames[start_ind] = frames[end_ind];
    start_ind++;
    end_ind++;
  }
  memset(frames+(end_let-moved), '.', moved);   //sets '.' at end of letters
  int more_moved = 0;
  while (ind != size-1) {
    if (frames[ind] == '.')
      ind++;
    else
      more_moved = defrag_helper(ind, end_let, moved);
  } 
  let_moved += more_moved;
  char c;
  vector<Proc>::iterator itr;
  char lets[26];
  int let_i =0;
  for (int i = temp_start; i < let_moved+temp_start; i++) {
    c = frames[i];
    while(frames[i] == c)
      i++; 
    lets[let_i] = c;
    let_i++;
  }
  cout << "time " << time+let_moved << "ms: Defragmentation complete (moved "
       << let_moved << " frames:"; 
  for (int x = 0; x<let_i; x++) {
    if (x<let_i-1)
      cout << " " << lets[x] << ",";
    else 
      cout << " " << lets[x] << ")" << endl;
  }
  partitions.clear();
  partitions.push_back(pair<int, int> (let_moved+temp_start, size-1));
  print();
  return let_moved;
}

int Memory::defrag_helper(int ind, int end_let, int moved){
  int start_ind=0, end_ind=ind, moved2=0, let_moved;
  start_ind = end_let - moved;
  while(frames[ind] != '.')  //goes to end of letters
    ind++;
  end_let = ind;
  let_moved = end_let-end_ind;
  while (start_ind != end_let - (end_ind - start_ind)) {
    frames[start_ind] = frames[end_ind];
    start_ind++;
    end_ind++;
  }
  moved2 = end_ind - start_ind;
  memset(frames+(end_let - moved2), '.', moved2);   //sets '.' at end of letters
  return let_moved;
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

//change times after defrag and mem start indexes
void Memory::change_times_mem(int& offset){
  int start = 0, i=0;
  vector<Proc>::iterator itr = procs.begin();
  while (i < 255) {
    char c = frames[i];
    if (c == '.') break;
    start = i;
    while(c == frames[i])
      i++;
    for (itr = procs.begin(); itr!=procs.end(); itr++) {
      if(c == itr->name) {
        itr->mem_b = start;
        itr->arrival_t += offset;
        itr->exit_t +=offset;
      }
    }
  }
}

//function to end sim
int Memory::end_sim(int& time, int& arrival_t){
  int max = 0;
  vector<Proc>::iterator itr = procs.begin();
  while (!procs.empty()){
    max = procs.back().exit_t;
    complete(time, arrival_t);
  }
  return max;
}

int Memory::non_end_sim(int& time, int& arrival_t){
  int max = 0;
  vector<Proc>::iterator itr = procs.begin();
  while (!procs.empty()){
    max = procs.back().exit_t;
    non_complete(time, arrival_t);
  }
  return max;
}

void Memory::non_complete (int& time, int& arrival_t) {
  vector<Proc>::iterator itr, temp;
  int i = 0;
  for (itr = procs.begin(); itr != procs.end(); ++itr) {
    if (arrival_t > itr->exit_t) {
      int total = itr->memory;
      cout << "time " << itr->exit_t << "ms: Process " << itr->name
           << " removed from physical memory" << endl;
      i = 0;
      while (total > 0) {
        if (frames[i] == itr->name) {
          frames[i] = '.';
          total--;
        }
        i++;
      }
      empty += itr->memory;
      temp = itr;
      itr--;
      procs.erase(temp);
      print();
    }
  }
}

void Memory::non_add (Proc& p, int& time) {
  vector<Proc>::iterator itr;
  time = p.arrival_t;
  cout << "time " << time << "ms: Process " << p.name << " arrived (requires "
       << p.memory << " frames of physical memory)" << endl;
  if (p.memory > empty) {
    cout << "time " << time << "ms: Cannot place process " << p.name << " -- "
         << "skipping process " << p.name << endl;
    return;
  }
  else {
    int total = p.memory;
    int i = 0;
    while (total != 0) {
      if (frames[i] == '.') {
        frames[i] = p.name;
        total--;
      }
      i++;
    }
    empty -= p.memory;
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
