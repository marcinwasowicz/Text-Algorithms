#include "Trie.h"
#include<iostream>

int main(){
    string s = "ThisIsMySuffixTrieImplementation";
    string test1 = "This";
    string test2 = "sIsMy";
    string test3 = "SuffixTrie";
    string test4 = "sIsMY";
    string test5 = "SuffixTrik";
    string test6 = "Implementatiok";

    Trie trie(s);

    cout<<trie.factorin(test1)<<endl;
    cout<<trie.factorin(test2)<<endl;
    cout<<trie.factorin(test3)<<endl;
    cout<<trie.factorin(test4)<<endl;
    cout<<trie.factorin(test5)<<endl;
    cout<<trie.factorin(test6)<<endl;

    return 0;
}