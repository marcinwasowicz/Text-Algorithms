#include<iostream>
#include<map>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<stack>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ll long long int
using namespace std;
/*
Program works as follows: Firstly it creates a dictionary in which count of every char in the file is stored. This dictionary
is then used to create huffman tree and coding dictionary, huffman tree is serialized into a binary file. Coding dictionary
maps characters into arrays of booleans. During input file compression, every boolean from an array representing some character
is stored in one bit in compressed version. During decompression huffman tree is deserialized, and used as a finite automaton
accepting single bits from compressed file. Coding dictionary could also be used to decompress file, but it would be an inefficient
solution when size of an alphabet is big (ex. text written in Chinese).
*/

struct node{
    char letter;
    ll weight;
    node* left;
    node* right;
};

node* makeNode(char letter, ll weight, node* left, node* right){
    node* res = new node;
    res->letter = letter;
    res->weight = weight;
    res->left = left;
    res->right = right;
    return res;
}

bool isLeaf(node* n){
    return n->left == NULL && n->right == NULL;
}

unordered_map<char, ll> getWeights(FILE* file){
    unordered_map<char, ll> result;
    for(char c = fgetc(file); c!=EOF; c = fgetc(file)){
        result[c]++;
    }
    return result;
}

node* makeTree(unordered_map<char, ll>& weights){
    multimap<ll, node*> priorityQueue;
    for(auto it : weights){
        priorityQueue.insert(make_pair(it.second, makeNode(it.first, it.second, NULL, NULL)));
    }

    while(priorityQueue.size() > 1){
        auto node1 = priorityQueue.begin();
        priorityQueue.erase(node1);
        auto node2 = priorityQueue.begin();
        priorityQueue.erase(node2);
        node* newRoot = makeNode('_', node1->first + node2->first, node1->second, node2->second);
        priorityQueue.insert(make_pair(newRoot->weight, newRoot));
    }
    return priorityQueue.begin()->second;
}

void deleteTree(node* root){
    if(root == NULL){
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void makeCodingDictionary(node* huffmanTree, unordered_map<char, vector<bool>>& dict, vector<bool>& workspace){
    if(isLeaf(huffmanTree)){
        dict[huffmanTree->letter] = workspace;
        return;
    }
    workspace.push_back(false);
    makeCodingDictionary(huffmanTree->left, dict, workspace);
    workspace.pop_back();
    workspace.push_back(true);
    makeCodingDictionary(huffmanTree->right, dict, workspace);
    workspace.pop_back();
}

node* getHuffmanCoding(unordered_map<char, vector<bool>>& dict,FILE* file){
    unordered_map<char, ll> weights = getWeights(file);
    node* root = makeTree(weights);
    vector<bool> workspace;
    makeCodingDictionary(root, dict, workspace);
    return root;
}

void serializeHuffmanTree(node* root, FILE* file){
    if(root == NULL){
        ll d = 0;
        fwrite(&d, sizeof(ll), 1, file);
        char c = '_';
        fwrite(&c, sizeof(char), 1, file);
        return;
    }
    fwrite(&(root->weight), sizeof(ll), 1, file);
    fwrite(&(root->letter), sizeof(char), 1, file);
    serializeHuffmanTree(root->left, file);
    serializeHuffmanTree(root->right, file);
}

node* deserializeHuffmanTree(FILE* file){
    ll weight;
    char letter;
    fread(&weight, sizeof(ll), 1, file);
    fread(&letter, sizeof(char), 1, file);
    if(weight == 0){
        return NULL;
    }
    node* root = makeNode(letter, weight, NULL, NULL);
    root->left = deserializeHuffmanTree(file);
    root->right = deserializeHuffmanTree(file);
    return root;
}

void compressFile(char* fileName){
    FILE* input = fopen(fileName, "r");
    unordered_map<char, vector<bool>> dict;
    node* huffmanTreeRoot = getHuffmanCoding(dict, input);
    fclose(input);
    FILE* serializedTree = fopen("huffmanTree", "w+");
    serializeHuffmanTree(huffmanTreeRoot, serializedTree);
    fclose(serializedTree);
    deleteTree(huffmanTreeRoot);
    char outputPath[256];
    FILE* output = fopen("compressed", "w+");
    input = fopen(fileName, "r");
    int biteCount = 0;
    char c = fgetc(input);
    vector<bool>::iterator it = dict[c].begin();
    char biteHolder;
    for(;c!=EOF;){
        for(;biteCount<8 && it!=dict[c].end(); biteCount++, it++){
            if(*it){
                biteHolder |= 1UL << biteCount;
            }
            else{
                biteHolder &= ~(1UL << biteCount);
            }
        }
        if(biteCount == 8){
            fwrite(&biteHolder, sizeof(char), 1, output);
            biteCount = 0;
        }
        if(it == dict[c].end()){
            c = fgetc(input);
            if(c!=EOF){
                it = dict[c].begin();
            }
        }
    }
    fwrite(&biteHolder, sizeof(char), 1, output);
    fclose(output);
    fclose(input);
}

void decompressFile(char* fileName){
    FILE* compressedVersion = fopen(fileName, "r");
    FILE* decompressedVersion = fopen("decompressed", "w+");
    FILE* tree = fopen("huffmanTree", "r");
    node* huffmanTree = deserializeHuffmanTree(tree);
    fclose(tree);
    int charsCount = 0;
    node* temp = huffmanTree;
    char c = fgetc(compressedVersion);
    int biteCount = 0;
    for(;c!=EOF && charsCount < huffmanTree->weight;){
        for(;biteCount<8 && !isLeaf(temp); biteCount++){
            bool b = (c >> biteCount) & 1;
            if(b){
                temp = temp->right;
            }
            else{
                temp = temp->left;
            }
        }
        if(isLeaf(temp)){
            fwrite(&(temp->letter), sizeof(char), 1, decompressedVersion);
            temp = huffmanTree;
            charsCount++;
        }
        if(biteCount == 8){
            biteCount = 0;
            c = fgetc(compressedVersion);
        }
    }
    deleteTree(huffmanTree);
    fclose(compressedVersion);
    fclose(decompressedVersion);
}

int main(int argc, char* argv[]){
    compressFile(argv[1]);
    char compressed[] = "compressed";
    decompressFile(compressed);
}