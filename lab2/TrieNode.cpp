#include "TrieNode.h"

TrieNode::TrieNode(TrieNode* link)
{
    if(link == NULL)
    {
        this->link = this;
    }
    else
    {
        this->link = link;
    }
}

TrieNode::~TrieNode()
{}

bool TrieNode::hasChild(char c)
{
    return this->children.find(c)!=this->children.end();
}

void TrieNode::addLink(char c, TrieNode* link)
{
    this->children[c] = link;
}
