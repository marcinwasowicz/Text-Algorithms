#include<iostream>
#include<string>
#include<vector>
#include<time.h>
using namespace std;

vector<int> prefixTable(string& pattern){
    clock_t start = clock();
    vector<int> result(pattern.length(), 0);
    int lengthS = 0;
    result[0] = 0;
    for(int iter = 1; iter<pattern.length();){
        if(pattern[iter] == pattern[lengthS]){
            lengthS++;
            result[iter] = lengthS;
            iter++;
        }
        else{
            if(lengthS!=0){
                lengthS = result[lengthS-1];
            }
            else{
                result[iter] = 0;
                iter++;
            }
        }
    }
    clock_t end = clock();
    cout<<"creating prefix table took "<<(end-start)/CLOCKS_PER_SEC<<endl;
    return result;
}

vector<int> KMP(string& text, string& pattern){
    vector<int> prefixT = prefixTable(pattern);
    clock_t start = clock();
    vector<int> result;
    for(int i = 0, j = 0; i<text.length();){
        if(pattern[j] == text[i]){
            i++;
            j++;
        }
        if(j == pattern.length()){
            result.push_back(i-j);
            j = prefixT[j-1];
        }
        else if(i<text.length() && pattern[j] != text[i]){
            if(j!=0){
                j = prefixT[j-1];
            }
            else{
                i++;
            }
        }
    }
    clock_t end = clock();
    cout<<"counting occurences took "<<(end-start)/CLOCKS_PER_SEC<<endl;
    return result;
}

int main(){
    string text;
    cin>>text;
    string pattern;
    cin>>pattern;
    vector<int> shifts = KMP(text, pattern);
    cout<<"occurences\n";
    for(int i = 0; i<shifts.size(); i++){
        cout<<shifts[i]<<" ";
    }
    cout<<"\noccurences\n";
    cout<<"number of occurences "<<shifts.size()<<endl;
}


