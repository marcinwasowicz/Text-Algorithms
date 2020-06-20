#include <iostream>
#include "Automaton.h"

int main() {

    vector<string> regexPatterns = {
            "", // empty string case ("" is NOT valid empty string initialization in c++)
            "a",
            "aaa*",
            "a+b?",
            "aa+[bcd]b?",
            "bab+a*.a[nhu]a*",
            "cccg+a?bb*v[op]"
    };

    vector<vector<pair<string, bool>>> tests = {
            {make_pair("", true), make_pair("whatever", false)},

            {make_pair("a", true), make_pair("aaaaa", false)},

            {make_pair("aa", true), make_pair("aaaaaa", true), make_pair("aaab", false), make_pair("a", false)},

            {make_pair("a", true), make_pair("aaaaaaa", true),make_pair("aaaaab", true),make_pair("aaabbbbb", false),
             make_pair("aaaaabbbfffbbg", false)},

            {make_pair("abb", false), make_pair("aabb", true), make_pair("aaaaaabb", true),make_pair("aaacc",false),
             make_pair("aaaabbb", false), make_pair("aaacb", true),make_pair("aaaab", true), make_pair("aaaa", false)},

            {make_pair("baacna", false), make_pair("babbbaaahaaa", true), make_pair("babbbxau", true), make_pair("babbaaacnaaa", false)},

            {make_pair("ccgbbbbbvp", false), make_pair("cccbbbvp", false), make_pair("cccggggbbbvo", true), make_pair("cccgggabbbvp", true),
             make_pair("cccgggabvpp", false), make_pair("cccgggggabbbbbvo", true), make_pair("cccggggaabbbbbbvp", false), make_pair("cccgabvo", true),
             make_pair("cccgbvo", true)}
    };

    for(int i = 0; i< regexPatterns.size(); i++){
        Automaton automaton(regexPatterns[i]);
        cout<<"running tests for regular expression no. "<<i<<" "<<regexPatterns[i]<<": "<<endl;
        for(int j = 0; j<tests[i].size(); j++){
            if(automaton.match(tests[i][j].first) == tests[i][j].second){
                cout<<"test no. "<<j<<" PASSED"<<endl;
            }
            else{
                cout<<"test no. "<<j<<" FAILED"<<endl;
                cout<<"INFO: input was "<<tests[i][j].first<<" and verdict was "<<tests[i][j].second<<endl;
            }
        }
    }

    return 0;
}
