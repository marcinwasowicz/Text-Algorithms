#include<unordered_map>
using namespace std;

class TrieNode
{
    public:
        unordered_map<char, TrieNode*> children;
        TrieNode* link;
    public:
        TrieNode(TrieNode* node = NULL);
        ~TrieNode();
        bool hasChild(char c);
        void addLink(char c, TrieNode* link);
};