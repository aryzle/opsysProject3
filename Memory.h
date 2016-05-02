using namespace std;

class Memory {
  public:
    Memory ();
    Memory (string);
    int size, empty;
    char frames[256];

    void print() const;
};
