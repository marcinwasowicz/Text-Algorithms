#include "Automaton.h"

class PatternSearch{
private:
    Automaton* automaton;
    vector<int> parseLine(vector<int>& line);
public:
    PatternSearch(vector<string> &pattern, string &alphabet);
    ~PatternSearch();
    vector<pair<int, vector<int>>> findPattern(vector<string>& text);
};
