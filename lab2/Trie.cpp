#include "Trie.h"

Trie::Trie(string& text)
{
    this->root = new TrieNode();
    TrieNode* workingLeaf = new TrieNode(this->root);

    this->root->addLink(text[0], workingLeaf);
    for(int i = 1; i<text.length(); i++)
    {
        TrieNode* current = workingLeaf;
        TrieNode* previous = NULL;
        while(!current->hasChild(text[i]))
        {
            TrieNode* temp = new TrieNode();
            current->addLink(text[i], temp);
            if(previous!=NULL)
            {
                previous->link = temp;
            }
            previous = temp;
            current = current->link;
        }
        if(current == this->root)
        {
            previous->link = this->root;
        }
        else
        {
            previous->link = current->children[text[i]];
        }
        workingLeaf = workingLeaf->children[text[i]];
    }
}

Trie::~Trie()
{
    this->deleteTree(this->root);
}

void Trie::deleteTree(TrieNode* root)
{
    if(root == NULL)
    {
        return;
    }
    for(auto child : root->children)
    {
        deleteTree(child.second);
    }
    delete root;
}

bool Trie::factorin(string& pattern)
{
    TrieNode* it = this->root;
    for(int i = 0; i<pattern.length(); i++)
    {
        if(!it->hasChild(pattern[i]))
        {
            return false;
        }
        it = it->children[pattern[i]];
    }
    return true;
}