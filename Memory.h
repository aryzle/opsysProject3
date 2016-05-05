#include <vector>
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

    void print() const;
    void add(Proc p);
    bool check(int mem);
    void defrag();
};
