#include "Automaton.h"
#include <queue>
#include <stack>
#include <unordered_set>

int Automaton::readChar(char letter) {
    while(this->currentState->transitions.find(letter) == this->currentState->transitions.end()){
        this->currentState = this->currentState->fail;
    }
    this->currentState = this->currentState->transitions[letter];
    return this->currentState->stateID;
}

void Automaton::rollBack() {
    this->currentState = this->trie;
}

Automaton::Automaton(vector<string>& pattern, string& alphabet) {
    int stateIDCounter = 0;
    this->trie = new node;
    this->trie->stateID = stateIDCounter;
    stateIDCounter++;

    this->currentState = this->trie;

    //firstly we build trie

    for(auto column : pattern){
        node* temp = this->trie;
        for(auto letter : column){
            if(temp->transitions.find(letter) == temp->transitions.end()){
                temp->transitions[letter] = new node;
                temp->transitions[letter]->stateID = stateIDCounter;
                stateIDCounter++;
            }
            temp = temp->transitions[letter];
        }
    }

    // now construct automaton ;)

    queue<node*> Q;

    for(auto letter : alphabet){
        if(this->trie->transitions.find(letter) != this->trie->transitions.end()){
            this->trie->transitions[letter]->fail = this->trie;
            Q.push(this->trie->transitions[letter]);
        }
        else{
            this->trie->transitions[letter] = this->trie;
        }
    }

    while(!Q.empty()){
        node* vertex = Q.front();
        Q.pop();
        for(auto letter : alphabet){
            if(vertex->transitions.find(letter) != vertex->transitions.end()){
                node* next = vertex->transitions[letter];
                Q.push(next);
                node* temp = vertex->fail;
                while(temp->transitions.find(letter) == temp->transitions.end()){
                    temp = temp->fail;
                }
                next->fail = temp->transitions[letter];
            }
        }
    }

    // finaly w compute final states of automaton after reading each pattern column and create classic finite automaton
    // for this states.

    this->computeFinalStates(pattern);
    this->computeFinalStatesAutomaton();

}

void Automaton::computeFinalStates(vector<string>& pattern) {
    // simply pass each column of pattern through automaton, and store final state
    for(auto column : pattern){
        this->finalStates.push_back(0);
        for(auto letter : column){
            this->finalStates[this->finalStates.size()-1] = this->readChar(letter);
        }
        this->rollBack();
    }
}

void Automaton::computeFinalStatesAutomaton() {
    // compute classic finite automaton for final states
    for(int i = 0; i<this->finalStates.size(); i++){
        if(this->finalStatesAutomaton.find(this->finalStates[i])== this->finalStatesAutomaton.end()){
            this->finalStatesAutomaton[this->finalStates[i]] = vector<int> (this->finalStates.size()+1, 0);
        }
    }
    int longPS = 0;
    this->finalStatesAutomaton[this->finalStates[0]][0] = 1;
    for(int i = 1; i<=this->finalStates.size(); i++){
        for(auto it = this->finalStatesAutomaton.begin(); it!=this->finalStatesAutomaton.end(); it++){
            this->finalStatesAutomaton[it->first][i] = this->finalStatesAutomaton[it->first][longPS];
        }
        if(i<this->finalStates.size()){
            this->finalStatesAutomaton[this->finalStates[i]][i] = i+1;
            longPS = this->finalStatesAutomaton[this->finalStates[i]][longPS];
        }
    }
}

vector<int> Automaton::getFinalStates() {
    return this->finalStates;
}

unordered_map<int, vector<int>> Automaton::getFinalStatesAutomaton() {
    return this->finalStatesAutomaton;
}

Automaton::~Automaton() {
    stack<node*> S;
    unordered_set<node*> trash;
    S.push(this->trie);
    while(!S.empty()){
        node* vertex = S.top();
        S.pop();
        for(auto child : vertex->transitions){
            if(child.second == vertex || trash.find(child.second) != trash.end()){
                continue;
            }
            S.push(child.second);
        }
        trash.insert(vertex);
    }

    for(auto vertex : trash){
        delete vertex;
    }
}