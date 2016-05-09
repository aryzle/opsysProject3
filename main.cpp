/* Op Sys project 3
  Memory Management Simulations
  
  Dylan Shute
  Arya Seghatoleslami
  
*/


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Memory.h"
#include "Proc.h"
#include "deque.h"

void run (deque<Proc> &procs, int n, string scheme, string algo);
void create_proc (string line, deque<Proc> &p);
void opt(int frames[], vector<int> &pages, int &num_pages, int &F);
void lru(int frames[], vector<int> &pages, int &F);
void lfu(int frames[], vector<int> &pages, int &F);
void out(int frames[], int &F);

int main (int argc, char* argv[]) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " input" << endl;
    return 1;
  }
  string line;
  deque<Proc> procs_cont;
  deque<Proc> procs_ncont;
  deque<Proc> procs_cont2;
  deque<Proc> procs_cont3;

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
  //added this for virtual memory 
  ifstream in_str;
  in_str.open(argv[2], ifstream::in);
  string line2;
  vector<int> pages;
  int num_pages = 0;
  
  while (in_str){
      if(!getline(in_str,line2)) break;
      
      istringstream stream(line2);
      int value;
      while (stream >> value){
        pages.push_back(value);
        num_pages++;
      }    
  }
  
  int F = 3;
  int frames[F];
  int i=0;
  for ( ;i<F; i++){
      frames[i] = '.';
  }
  int frames2[F];
  int frames3[F];
  copy(frames,frames + F, frames2);
  copy(frames,frames + F, frames3);

  procs_cont2 = procs_cont;
  procs_cont3 = procs_cont;
  
  run(procs_cont, num_procs, "Contiguous", "First");
  run(procs_cont2, num_procs, "Contiguous", "Next");
  run(procs_cont3, num_procs, "Contiguous", "Best");

  run(procs_ncont, num_procs, "Non-contiguous", "");
  
  opt(frames3, pages, num_pages, F);
  //cout << endl;
  lru(frames, pages, F);
  //cout << endl;
  lfu(frames2, pages, F);
  return 0;
}

void run (deque<Proc> &procs, int n, string scheme, string algo) {
  Memory m = Memory(); 
  Proc p;
  int time = 0;
  int offset = 0;
  const int t_memmove = 1;
  //m.print();
  
  if (scheme == "Contiguous"){
    cout << "time " << time << "ms: Simulator started (" << scheme << " -- "
           << algo << "-Fit)" << endl;
    if( algo == "First"){                          
      while (!procs.empty()) {
        p = procs.front();
        procs.pop_front();
        m.complete(time, p.arrival_t);
        offset = m.add(p, time, algo);
        if(offset > 0) {change_times(procs, offset);}
        m.print();
      }
      int over = 1000000;
      time = m.end_sim(time, over);
      
    }
    else if (algo == "Next"){
      while(!procs.empty()){
        p = procs.front();
        procs.pop_front();
        m.complete(time, p.arrival_t);
        offset = m.add(p,time, algo);
        if(offset>0) {change_times(procs, offset);}
        m.print();
      }
      int over = 1000000;
      time = m.end_sim(time, over);
    }
    else if (algo == "Best"){
      while(!procs.empty()){
        p = procs.front();
        procs.pop_front();
        m.complete(time, p.arrival_t);
        offset = m.add(p,time, algo);
        if(offset>0) {change_times(procs, offset);}
        m.print();
      }
      int over = 1000000;
      time = m.end_sim(time, over);
    }
    cout << "time " << time << "ms: Simulator ended (" << scheme << " -- " << algo << "-Fit)" << endl;
  }
  else if(scheme ==  "Non-contiguous"){
    //do non contiguous 
    cout << "time " << time << "ms: Simulator started (" << scheme << ")" << endl;
    while(!procs.empty()){
      p = procs.front();
      procs.pop_front();
      m.non_complete(time, p.arrival_t);
      m.non_add(p,time);
      m.print();
    }
    int over = 1000000;
    time = m.non_end_sim(time, over);
    
    cout << "time " << time << "ms: Simulator ended (" << scheme << ")"<< endl;
    
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

void out(int frames[], int &F){
  cout << " [mem:" ;
  for (int i = 0; i<F; i++){
      if (frames[i] == 46){               //ascii period = 46, keeping an int array
          cout << " .";
      }
      else 
          cout << " " << frames[i];
  }
  cout << "]";
}

void opt(int frames[], vector<int> &pages, int &num_pages, int &F){
  int num_page_faults = 0;    
  cout << "Simulating OPT with fixed frame size of " << F << endl;
  int cur_fs = 0;
  int ind = 0;
  int size = pages.size();
  int f_ind = 0;
  int victim;
  //keeps track of the index page was last used
  vector<int> page_track(100);
  while (ind < size){
      cout << "referencing page " << pages[ind];
      
      if (cur_fs == 0){                                       //starting sim,frames empty
          frames[f_ind] = pages[ind];
          out(frames, F);
          cout <<  " PAGE FAULT (no victim page)" << endl;
          cur_fs = 1;
          f_ind++;
          num_page_faults++;
      }
      else {                                                  
          int flag = 0;
          for (int x = 0; x<F; x++){                          //checks to see if page in frame
              if (pages[ind] == frames[x]){
                  out(frames, F);
                  cout << endl;
                  flag =1;
                  break;
              }
          }
          if(f_ind < F && flag == 0){                     //if frame not empty
              frames[f_ind] = pages[ind];
              out(frames, F);
              cout << " PAGE FAULT (no victim page)" <<endl;
              f_ind++;
              num_page_faults++;
          }
          else if (f_ind >= F && flag == 0){              //finds victim page by looking forward
              int v_ind = 0;
              int max_ind = 0;
              int i=0;
              while(i<F){
                  for(int x=ind; x<num_pages; x++){
                      if(pages[x] == frames[i]){
                          if(x > max_ind){
                              max_ind = x;
                              v_ind = i;
                          }
                          break;
                      }
                      else if ( x == num_pages-1){
                          if(max_ind == num_pages){
                              if (frames[i] < frames[v_ind] ){
                                  v_ind = i;
                              }
                          }
                          else {
                              max_ind = num_pages;
                              v_ind = i;
                          }
                      }
                  }
                  i++;
              }
              if (ind == num_pages-1){
                  int min = frames[0];
                  v_ind = 0;
                  for(int x = 0; x<F; x++){
                      if (frames[x] < min) {
                          min = frames[x];
                          v_ind = x;
                      }
                  }
              }
              victim = frames[v_ind];
              frames[v_ind] = pages[ind];
              out(frames, F);
              cout << " PAGE FAULT (victim page " << victim << ")" << endl; 
              num_page_faults++;
          }
      }
      
      page_track[pages[ind]] = ind;
      ind++;
  }
  
  cout << "End of OPT simulation (" << num_page_faults << " page faults)" << endl;
}

void lru(int frames[], vector<int> &pages, int &F){
  int num_page_faults = 0;    
  cout << "Simulating LRU with fixed frame size of " << F << endl;
  int cur_fs = 0;
  int ind = 0;
  int size = pages.size();
  int f_ind = 0;
  int victim;
  //keeps track of the index page was last used
  vector<int> page_track(100);
  while (ind < size){
      cout << "referencing page " << pages[ind];
      
      if (cur_fs == 0){                                       //starting sim,frames empty
          frames[f_ind] = pages[ind];
          out(frames, F);
          cout <<  " PAGE FAULT (no victim page)" << endl;
          cur_fs = 1;
          f_ind++;
          num_page_faults++;
      }
      else {                                                  
          int flag = 0;
          for (int x = 0; x<F; x++){                          //checks to see if page in frame
              if (pages[ind] == frames[x]){
                  out(frames, F);
                  cout << endl;
                  flag =1;
                  break;
              }
          }
          if(f_ind < F && flag == 0){                     //if frame not empty
              frames[f_ind] = pages[ind];
              out(frames, F);
              cout << " PAGE FAULT (no victim page)" <<endl;
              f_ind++;
              num_page_faults++;
          }
          else if (f_ind >= F && flag == 0){                                   //finds victim page by least recently used
              int min;
              int min_ind = 0;
              min = page_track[frames[0]];
              for (int x = 1; x<F; x++){ 
                  if (page_track[frames[x]] < min) {
                      min = page_track[frames[x]];
                      min_ind = x;
                  }   
              }
              victim = frames[min_ind];
              frames[min_ind] = pages[ind];
              out(frames, F);
              cout << " PAGE FAULT (victim page " << victim << ")" << endl; 
              num_page_faults++;
          }
      }
      
      page_track[pages[ind]] = ind;
      ind++;
  }
  
  cout << "End of LRU simulation (" << num_page_faults << " page faults)" << endl;
}

void lfu(int frames[], vector<int> &pages, int &F){
  int num_page_faults = 0;    
  cout << "Simulating LFU with fixed frame size of " << F << endl;
  int cur_fs = 0;
  int ind = 0;
  int size = pages.size();
  int f_ind = 0;
  int victim;
  //keeps track of the index page was last used
  vector<int> page_track(100);
  while (ind < size){
      cout << "referencing page " << pages[ind];
      
      if (cur_fs == 0){                                       //starting sim,frames empty
          frames[f_ind] = pages[ind];
          out(frames, F);
          cout <<  " PAGE FAULT (no victim page)" << endl;
          cur_fs = 1;
          f_ind++;
          num_page_faults++;
      }
      else {                                                  
          int flag = 0;
          for (int x = 0; x<F; x++){                          //checks to see if page in frame
              if (pages[ind] == frames[x]){
                  out(frames, F);
                  cout << endl;
                  flag =1;
                  break;
              }
          }
          if(f_ind < F && flag == 0){                     //if frame not empty
              frames[f_ind] = pages[ind];
              out(frames, F);
              cout << " PAGE FAULT (no victim page)" <<endl;
              f_ind++;
              num_page_faults++;
          }
          else if (f_ind >= F && flag == 0){                  //finds victim page by least frequently used
              int min;
              int min_ind = 0;
              min = page_track[frames[0]];
              for (int x = 1; x<F; x++){ 
                  if (page_track[frames[x]] < min) {
                      min = page_track[frames[x]];
                      min_ind = x;
                  }  
                  else if (page_track[frames[x]] == min){
                      if(frames[x] < frames[min_ind]){
                          min = page_track[frames[x]];
                          min_ind = x;
                      }
                  }
              }
              victim = frames[min_ind];
              page_track[victim] = 0;
              frames[min_ind] = pages[ind];
              out(frames, F);
              cout << " PAGE FAULT (victim page " << victim << ")" << endl; 
              num_page_faults++;
          }
      }
      
      page_track[pages[ind]]++;
      ind++;
  }
  cout <<"End of LFU simulation (" << num_page_faults << " page faults)" << endl;
}
