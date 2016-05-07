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
    int add (Proc& p, int& time);
    int check (const Proc p, const int time);
    int defrag (int time, Proc p);
    void complete (int& time, int arrival_t);
    void change_times_mem (int& offset);
};
