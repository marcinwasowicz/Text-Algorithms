#include "TrieNode.h"

class Trie
{
    private:
        TrieNode* root;
        void deleteTree(TrieNode* node);
    public:
        Trie(string& s);
        ~Trie();
        bool factorin(string& pattern);
};

