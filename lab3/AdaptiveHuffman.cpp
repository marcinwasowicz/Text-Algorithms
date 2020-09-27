#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unordered_map>
#include<map>
#include<string>
#include<vector>

#define ll long long int
#define NYT_CHAR '\0'
#define NOT_IMPORTANT_CHAR '_'
#define MY_MAP unordered_map<ll, map<int, node*>>
using namespace std;

struct node{
    char letter;
    ll weight;
    int index;
    node* left;
    node* right;
    node* parent;
};

node* makeNode(char letter, ll weight,int index, node* left, node* right, node* parent){
    node* result = new node;
    result->letter = letter;
    result->weight = weight;
    result->left = left;
    result->right = right;
    result->parent = parent;
    result->index = index;
    return result;
}

void swapNodesAndIncrement(node* a, node* b,MY_MAP& weightClasses){
    weightClasses[a->weight].erase(a->index);
    weightClasses[b->weight].erase(b->index);
    int temp = a->index;
    a->index = b->index;
    b->index = temp;
    a->weight++;
    weightClasses[a->weight][a->index] = a;
    weightClasses[b->weight][b->index] = b;
    node* parentB = b->parent;
    node* parentA = a->parent;
    if(parentB->left == b){
        parentB->left = a;
    }
    else{
        parentB->right = a;
    }
    if(parentA->left == a){
        parentA->left = b;
    }
    else{
        parentA -> right = b;
    }
    b->parent = parentA;
    a->parent = parentB;
}

void increment(node* n, MY_MAP& weightClasses){
    weightClasses[n->weight].erase(n->index);
    n->weight++;
    weightClasses[n->weight][n->index] = n;
}
bool shouldSwap(node* n,MY_MAP& weightClasses){
    auto it = weightClasses[n->weight].end();
    it--;
    return n != it->second;
}

node* getHighestInClass(node* n, MY_MAP& weightClasses){
    auto it = weightClasses[n->weight].end();
    it--;
    return it->second;
}

void swapAndIncrement(node* n, MY_MAP& weightClasses){
    node* temp = getHighestInClass(n, weightClasses);
    swapNodesAndIncrement(n, temp, weightClasses);
}

node* splitNYT(char letter, node*& nyt, MY_MAP& weightClasses){
    node* left = makeNode(NYT_CHAR, 0, nyt->index-2, NULL, NULL, nyt);
    node* right = makeNode(letter, 1,nyt->index-1, NULL, NULL, nyt);
    nyt->letter = NOT_IMPORTANT_CHAR;
    nyt->left = left;
    nyt->right = right;
    weightClasses[0][nyt->index] = nyt;
    weightClasses[1][right->index] = right;
    nyt = nyt->left;
    return right;
}

void updateTree(node* root,node*& nyt, unordered_map<char, node*>& letters, char letter, MY_MAP& weightClasses){
    node* n;
    if(letters.find(letter)!=letters.end()){
        n = letters[letter];
    }
    else{
        n = splitNYT(letter, nyt, weightClasses);
        letters[letter] = n;
    }
    n = n->parent;
    while(n!=NULL){
        if(shouldSwap(n, weightClasses)){
            swapAndIncrement(n, weightClasses);
        }
        else{
            increment(n, weightClasses);
        }
        n = n->parent;
    }
}

node* initialize(int alphabetSize, MY_MAP& weightClasses){
    node* root = makeNode(NYT_CHAR, 0, 2*alphabetSize-1, NULL, NULL, NULL);
    weightClasses[root->weight][root->index] = root;
}

void deleteTree(node* root){
    if(root == NULL){
        return;
    }
    deleteTree(root->right);
    deleteTree(root->left);
    delete root;
}

vector<bool> getLetterCode(char letter, node* root,node* nyt, unordered_map<char, node*>& letters){
    vector<bool> result;
    node* n;
    if(letters.find(letter)!=letters.end()){
        n = letters[letter];
    }
    else{
        n = nyt;
        for(int i = 7; i>=0; i--){
            result.insert(result.begin(), (letter >> i)&1);
        }
    }
    while(n->parent!=NULL){
        if(n->parent->left == n){
            result.insert(result.begin(), false);
        }
        else{
            result.insert(result.begin(), true);
        }
        n = n->parent;
    }
    return result;
}

void compress(char* fileName, int alphabetSize){
    FILE* file = fopen(fileName, "r");
    FILE* compressed = fopen("compressed", "w+");
    MY_MAP weightClasses;
    unordered_map<char, node*> letters;
    node* root = initialize(alphabetSize, weightClasses);
    node* nyt = root;
    char c = fgetc(file);
    char biteHolder;
    int biteCount = 0;
    vector<bool> code = getLetterCode(c, root, nyt, letters);
    vector<bool>::iterator it = code.begin();
    for(;c!=EOF;){
        for(;biteCount<8 && it!=code.end(); biteCount++, it++){
            if(*it){
                biteHolder |= 1UL << biteCount;
            }
            else{
                biteHolder &= ~(1UL << biteCount);
            }
        }
        if(biteCount == 8){
            fwrite(&biteHolder, sizeof(char), 1, compressed);
            biteCount = 0;
        }
        if(it == code.end()){
            updateTree(root, nyt, letters, c, weightClasses);
            c = fgetc(file);
            if(c!=EOF){
                code = getLetterCode(c, root, nyt, letters);
                it = code.begin();
            }
        }
    }
    fwrite(&biteHolder, sizeof(char), 1, compressed);
    FILE* huffmanTree = fopen("huffmanTree", "w+");
    fwrite(&(root->weight), sizeof(ll), 1, huffmanTree);
    fclose(huffmanTree);
    deleteTree(root);
    fclose(file);
    fclose(compressed);
}

bool isLeaf(node* n){
    return n->left == NULL && n->right == NULL;
}

void decompress(int alphabetSize){
    FILE* input = fopen("compressed", "r");
    FILE* huffmanTree = fopen("huffmanTree", "r");
    FILE* output = fopen("decompressed", "w+");
    ll maxCount;
    fread(&maxCount, sizeof(ll), 1, huffmanTree);
    fclose(huffmanTree);
    ll charCount = 0;
    MY_MAP weightClasses;
    unordered_map<char, node*> letters;
    node* root = initialize(alphabetSize, weightClasses);
    node* nyt = root;
    int biteCount = 0;
    node* temp = root;
    char c = fgetc(input);
    for(;c!=EOF && charCount < maxCount;){
        if(nyt == root){
            fwrite(&c, sizeof(char), 1, output);
            charCount++;
            updateTree(root, nyt, letters,c, weightClasses);
            c = fgetc(input);
            continue;
        }
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
            if(temp!=nyt){
                fwrite(&(temp->letter), sizeof(char), 1, output);
                updateTree(root, nyt, letters, temp->letter, weightClasses);
            }
            else{
                char workspace;
                for(int i = 0; i<8; i++, biteCount++){
                    if(biteCount == 8){
                        biteCount = 0;
                        c = fgetc(input);
                    }
                    bool b = (c >> biteCount) & 1;
                    if(b){
                        workspace |= 1UL << i;
                    }
                    else{
                        workspace &= ~(1UL << i);
                    }
                }
                fwrite(&workspace, sizeof(char), 1, output);
                updateTree(root, nyt, letters,workspace,weightClasses);
            }
            temp = root;
            charCount++;
        }
        if(biteCount == 8){
            biteCount = 0;
            c = fgetc(input);
        }
    }
    fclose(input);
    fclose(output);
    deleteTree(root);
}

int main(int argc, char* argv[]){
    compress(argv[1], 256);
    decompress(256);
    return 0;
}


