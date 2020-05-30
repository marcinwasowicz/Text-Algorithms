#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

struct node{
    int stateID;
    node* fail;
    unordered_map<char, node*> transitions;
};

class Automaton{
private:
    node* currentState;
    node* trie;
    vector<int> finalStates;
    unordered_map<int, vector<int>> finalStatesAutomaton;
private:
    void computeFinalStates(vector<string>& pattern);
    void computeFinalStatesAutomaton();
public:
    int readChar(char letter);
    void rollBack();
    Automaton(vector<string>& pattern, string& alphabet);
    vector<int> getFinalStates();
    unordered_map<int, vector<int>> getFinalStatesAutomaton();
    ~Automaton();
};