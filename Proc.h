#include <vector>
using namespace std;

class Proc {
  public:
    Proc ();
    Proc (char, vector<int>);
    char name;
    int memory, arrival_t, exit_t;

    void print() const;
};
