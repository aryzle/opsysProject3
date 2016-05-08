#include <vector>
#include <deque>
using namespace std;

class Proc;

class Memory {
  public:
    Memory ();
    Memory (string);
    int size, empty;
    char frames[256];
    vector< pair<int, int> > partitions;
    vector<Proc> procs;
  

    void print () const;
    int add (Proc& p, int& time, string& algo);
    int check (const Proc p, const int time, string &algo);
    //int best_check (const Proc p, const int time);
    //int next_check (const Proc p, const int time);
    int defrag (int time, Proc p);
    int defrag_helper(int ind, int end_let, int moved);
    void complete (int& time, int arrival_t);
    void non_complete (int& time, int& arrival_t);
    void non_add(Proc& p, int& time);
    int non_check (const Proc p, const int time);
    void change_times_mem (int& offset);
    int end_sim(int& time, int& arrival_t);
    int non_end_sim(int& time, int& arrival_t);

};
