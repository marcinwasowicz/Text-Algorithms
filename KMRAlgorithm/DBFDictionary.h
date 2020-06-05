#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include "RadixSort.h"
using namespace std;

class DBFDictionary{
public:
    DBFDictionary(string& text);
    ~DBFDictionary();
    bool compareTwoFactors(int length, int factorPosition1, int factorPosition2);
    vector<int> findPattern(string& text, string& pattern);
    long long int getApproximatedSizeInBytes();
private:
    RadixSort* radixSort;
    unordered_map<int, vector<int>> dbf;
    unordered_map<int, vector<int>> positionsDict;
};