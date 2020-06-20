#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

struct node{
    vector<node*> epsilon;
    unordered_map<char, vector<node*>> transitions;
};

class Automaton{
private:
    node* startState;
    node* finalState;
public:
    Automaton(string& regex);
    ~Automaton();
    bool match(string& text);
};