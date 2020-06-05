#include<map>
#include "DBFDictionary.h"


DBFDictionary::DBFDictionary(string& text){
    this->radixSort = new RadixSort();
    map<char, int> lettersID;
    int id = 0;
    for(auto letter : text){
        lettersID[letter] = 0;
    }
    for(auto it : lettersID) {
        lettersID[it.first] = id;
        id++;
    }
    for(auto letter : text){
        this->dbf[1].push_back(lettersID[letter]);
    }
    vector<vector<int>> singleLettersPriorities;
    for(int i = 0; i<this->dbf[1].size(); i++){
        singleLettersPriorities.push_back({i, this->dbf[1][i]});
    }
    this->radixSort->sort(singleLettersPriorities, 0, id, 1, 1);
    for(auto tuple : singleLettersPriorities){
        this->positionsDict[1].push_back(tuple[0]);
    }
    for(int i = 2; i<=text.length(); i *= 2){
        vector<vector<int>> temp;
        int tempMax = 0;
        int tempMin = text.length();
        for(int j = 0; j+i-1<text.length(); j++){
            tempMax = max(tempMax, max(this->dbf[i/2][j], this->dbf[i/2][j+i/2]));
            tempMin = min(tempMin, min(this->dbf[i/2][j], this->dbf[i/2][j+i/2]));
            temp.push_back({j, this->dbf[i/2][j], this->dbf[i/2][j+i/2]});
        }
        this->radixSort->sort(temp, tempMin, tempMax, 1, 2);
        this->dbf[i] = vector<int>(temp.size(), 0);
        id  = 0;
        for(int j = 1; j<temp.size(); j++){
            if(temp[j][1]!=temp[j-1][1] || temp[j][2] != temp[j-1][2]){
                id++;
            }
            this->dbf[i][temp[j][0]] = id;
        }
        for(auto tuple : temp){
            this->positionsDict[i].push_back(tuple[0]);
        }
    }
}

DBFDictionary::~DBFDictionary() {
    delete this->radixSort;
}

bool DBFDictionary::compareTwoFactors(int length, int factorPosition1, int factorPosition2) {
    int power = 0;
    while(1 << power <= length) {power++;}
    power--;
    int validLength = 1 << power;
    return this->dbf[validLength][factorPosition1] == this->dbf[validLength][factorPosition2] and
    this->dbf[validLength][factorPosition1 + length - validLength] == this->dbf[validLength][factorPosition2 + length - validLength];
}

// implement this method:
vector<int> DBFDictionary::findPattern(string &text, string &pattern) {

    // firstly we find such factor length L that L = 2^i for some i such that length <= pattern.length()
    int validPower = 0;
    for(;1 << validPower <= pattern.length(); validPower++){}
    validPower--;
    int validFactorLength = 1 << validPower;
    vector<int> indexesToSearch = this->positionsDict[validFactorLength];

    // now we perform binary search over lists of starting indexes of factors of text of length equal to L, in order to find index I, such that
    // factor of text starting at this index is a prefix of our pattern
    int factorPosition = -1;
    int leftBorder = 0;
    int rightBorder = indexesToSearch.size()-1;

    while(leftBorder <= rightBorder){
        int idx = leftBorder + (rightBorder - leftBorder)/2;
        string patternPrefix = pattern.substr(0, validFactorLength);
        string textSubstring = text.substr(indexesToSearch[idx], validFactorLength);
        if(patternPrefix == textSubstring){
            factorPosition = idx;
            break;
        }
        if(patternPrefix > textSubstring){
            leftBorder = idx + 1;
        }
        else{
            rightBorder = idx - 1;
        }
    }

    vector<int> result;
    if(factorPosition == -1){
        return result;
    }

    vector<int> factorPriorities = this->dbf[validFactorLength];
    int referencePriority = factorPriorities[indexesToSearch[factorPosition]];

    for(int i = factorPosition; i>= 0 && factorPriorities[indexesToSearch[i]] == referencePriority; i--){
        if(indexesToSearch[i] + pattern.length() < text.length() && pattern == text.substr(indexesToSearch[i], pattern.length())){
            result.insert(result.begin(), indexesToSearch[i]);
        }
    }

    for(int i = factorPosition + 1; i<indexesToSearch.size() && factorPriorities[indexesToSearch[i]] == referencePriority; i++){
        if(indexesToSearch[i] + pattern.length() < text.length() && pattern == text.substr(indexesToSearch[i], pattern.length())){
            result.push_back(indexesToSearch[i]);
        }
    }

    return result;
}

// calculate size in bytes:
long long int DBFDictionary::getApproximatedSizeInBytes() {
    long long int size = 0;
    size += sizeof(DBFDictionary);
    for(auto it : this->dbf){
        size += sizeof(it);
        size += it.second.size()* sizeof(int);
    }
    for(auto it : this->positionsDict){
        size += sizeof(it);
        size += it.second.size()* sizeof(int);
    }
    return size;
}