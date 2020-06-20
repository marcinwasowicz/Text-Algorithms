#include "Automaton.h"

Automaton::Automaton(string &regex) {
    this->startState = new node;
    this->finalState = this->startState;
    node* workState = this->startState;
    unordered_set<char> specialChars = {']', '[', '*', '+','?', '.'};

    for(int position = 0; position<regex.length();position++){
        char magicLetter = regex[position-1];
        if(specialChars.find(regex[position]) == specialChars.end()){
            if(position+1 >= regex.length() || (regex[position+1]!='*' && regex[position+1]!='+' && regex[position+1] != '?')) {
                node *temp = new node;
                workState->transitions[regex[position]].push_back(temp);
                workState = temp;
                this->finalState = workState;
            }
        }
        else if(regex[position] == '*'){
            node* temp1 = new node;
            node* temp2 = new node;
            workState->epsilon.push_back(temp1);
            temp1->transitions[magicLetter].push_back(temp2);
            temp2->epsilon.push_back(temp1);
            temp2->epsilon.push_back(workState);
        }
        else if(regex[position] == '+'){
            node* temp1 = new node;
            node* temp2 = new node;
            node* temp3 = new node;
            workState->transitions[magicLetter].push_back(temp1);
            temp1->epsilon.push_back(temp2);
            temp2->transitions[magicLetter].push_back(temp3);
            temp3->epsilon.push_back(temp2);
            temp3->epsilon.push_back(temp1);
            workState = temp1;
            this->finalState = temp1;
        }
        else if(regex[position] == '?'){
            node* temp1 = new node;
            node* temp2 = new node;
            node* temp3 = new node;
            workState->epsilon.push_back(temp1);
            temp1->transitions[magicLetter].push_back(temp2);
            temp2->epsilon.push_back(temp3);
            workState->epsilon.push_back(temp3);
            workState = temp3;
            this->finalState = workState;
        }
        else if(regex[position] == '['){
            node* temp = new node;
            for(position++; regex[position] != ']'; position++){
                workState->transitions[regex[position]].push_back(temp);
            }
            workState = temp;
            this->finalState = workState;
        }
        else {
            node* temp = new node;
            workState->transitions['.'].push_back(temp);
            workState = temp;
            this->finalState = temp;
        }
    }
}

Automaton::~Automaton() {
    stack<node*> S;
    unordered_set<node*> D;
    S.push(this->startState);
    D.insert(this->startState);
    while(!S.empty()){
        node* v = S.top();
        S.pop();
        for(auto it1 : v->epsilon){
            if(D.find(it1) == D.end()){
                S.push(it1);
                D.insert(it1);
            }
        }
        for(auto it1 : v->transitions){
            for(auto it2 : it1.second){
                if(D.find(it2) == D.end()){
                    S.push(it2);
                    D.insert(it2);
                }
            }
        }
    }
    for(auto it : D){
        delete it;
    }
}

bool Automaton::match(string &text) {
    queue<pair<node*, int>> Q;
    Q.push(make_pair(this->startState, 0));
    int queueSize = Q.size();
    while(!Q.empty()){
        for(int i = 0; i<queueSize; i++){
            node* v = Q.front().first;
            int letter = Q.front().second;
            Q.pop();
            if(v == this->finalState && letter == text.length()){
                return true;
            }
            for(auto epsilonTransition : v->epsilon){
                Q.push(make_pair(epsilonTransition, letter));
            }
            if(letter < text.length() && v->transitions.find(text[letter])!=v->transitions.end()){
                for(auto letterTransition : v->transitions[text[letter]]){
                    Q.push(make_pair(letterTransition, letter+1));
                }
            }
            if(letter < text.length() && v->transitions.find('.')!=v->transitions.end()){
                for(auto dotTransition : v->transitions['.']){
                    Q.push(make_pair(dotTransition, letter+1));
                }
            }
        }
        queueSize = Q.size();
    }
    return false;
}