#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<time.h>
using namespace std;

unordered_map<char, vector<int>> GetTransitionFunction(string& pattern){
    clock_t start = clock();
    unordered_map<char, vector<int>> result;
    for(int i = 0; i<pattern.length(); i++){
        if(result.find(pattern[i])==result.end()){
            result[pattern[i]] = vector<int> (pattern.length()+1, 0);
        }
    }
    int longPS = 0;
    result[pattern[0]][0] = 1;
    for(int i = 1; i<=pattern.length(); i++){
        for(auto it = result.begin(); it!=result.end(); it++){
            result[it->first][i] = result[it->first][longPS];
        }
        if(i<pattern.length()){
            result[pattern[i]][i] = i+1;
            longPS = result[pattern[i]][longPS];
        }
    }
    clock_t end = clock();
    cout<<"creating transition function took "<<(end-start)/CLOCKS_PER_SEC<<endl;
    return result;
}

vector<int> FiniteAutomataAlgorithm(string& text, string& pattern){
    vector<int> result;
    unordered_map<char, vector<int>> transitionTable = GetTransitionFunction(pattern);
    clock_t start = clock();
    int state = 0;
    for(int i = 0; i<text.length(); i++){
        if(transitionTable.find(text[i])== transitionTable.end()){
            state = 0;
            continue;
        }
        state = transitionTable[text[i]][state];
        if(state == pattern.length()){
            result.push_back(i+1-pattern.length());
        }
    }
    clock_t end = clock();
    cout<<"counting occurences:  "<<(end-start)/CLOCKS_PER_SEC<<endl;
    return result;
}

int main(){
    string text;
    cin>>text;
    string pattern;
    cin>>pattern;
    vector<int> shifts = FiniteAutomataAlgorithm(text, pattern);
    cout<<"occurences\n";
    for(int i = 0; i<shifts.size(); i++){
        cout<<shifts[i]<<" ";
    }
    cout<<"\noccurences\n";
}

