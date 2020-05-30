#include "PatternSearch.h"
#include <algorithm>
using namespace std;

PatternSearch::PatternSearch(vector<string>& pattern, string &alphabet) {
    this->automaton =  new Automaton(pattern, alphabet);
}

vector<int> PatternSearch::parseLine(vector<int> &line) {
    // pass line of states thorugh final states automaton, and store every index at which we got into accepting state
    vector<int> result;
    vector<int> finalStates = this->automaton->getFinalStates();
    unordered_map<int, vector<int>> machine = this->automaton->getFinalStatesAutomaton();
    int numOfStates = finalStates.size();

    int state = 0;

    for(int i = 0; i<line.size(); i++){
        if(machine.find(line[i]) == machine.end()){
            state = 0;
            continue;
        }
        state = machine[line[i]][state];
        if(state == numOfStates){
            result.push_back(i);
        }
    }

    return result;
}

vector<pair<int, vector<int>>> PatternSearch::findPattern(vector<string> &text) {
    // pass each column of text through automaton, and store automaton state after reading each letter
    vector<pair<int,vector<int>>> result;
    int len = 0;
    vector<vector<int>> automatonOutput;
    for(auto word : text){
        len = max(len, (int)word.length());
        automatonOutput.push_back({});
    }

    for(int i = 0; i<len; i++){
        for(int j = 0; j<text.size(); j++){
            if(i<text[j].length()){
                automatonOutput[j].push_back(this->automaton->readChar(text[j][i]));
            }
        }
        this->automaton->rollBack();
    }
    // apply final state automaton for each line of aho corasick automaton output,
    // and for each line store indexes at which final state automaton got into final state
    // ( we will not store information about lines whose parseLine method output is empty)

    for(int i = 0; i<automatonOutput.size(); i++){
        vector<int> temp = this->parseLine(automatonOutput[i]);
        if(temp.size()!=0){
            result.push_back(make_pair(i, temp));
        }
    }

    return result;
}

PatternSearch::~PatternSearch() {
    delete this->automaton;
}