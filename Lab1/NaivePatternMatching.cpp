#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<vector>
#include<time.h>
using namespace std;
vector<int> NaivePatternMatching(string& text, string& pattern){
    vector<int> result;
    for(int i = 0; i<text.length()-pattern.length()+1; i++){
        if(pattern == text.substr(i, pattern.length())){
            result.push_back(i);
        }
    }
    return result;
}

int main(){
    string file;
    cin>>file;
    string pattern;
    cin>>pattern;
    string text;
    clock_t algoStart = clock();
    vector<int> shifts = NaivePatternMatching(text, pattern);
    clock_t algoEnd = clock();
    cout<<"time of execution: "<< (algoEnd - algoStart)/CLOCKS_PER_SEC<<endl;
    cout<<"occurences\n";
    for(int i = 0; i<shifts.size(); i++){
        cout<<shifts[i]<<" ";
    }
    cout<<"\noccurences\n";
    cout<<"number of occurences "<<shifts.size()<<endl;
}